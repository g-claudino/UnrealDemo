// Fill out your copyright notice in the Description page of Project Settings.

// this is based on: https://alfredbaudisch.com/blog/gamedev/unreal-engine-ue/unreal-engine-actors-transparent-block-camera-occlusion-see-through/
// all credits to Alfred Reinold Baudisch (https://github.com/alfredbaudisch)

#include "OcclusionAwarePlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Containers/Set.h"

AOcclusionAwarePlayerController::AOcclusionAwarePlayerController(){
    CapsulePercentageForTrace = 1.0f;
    DebugLineTraces = true;
    IsOcclusionEnabled = true;
}

void AOcclusionAwarePlayerController::OnPossess(APawn* pawn){
    Super::OnPossess(pawn);

    if(!IsValid(pawn)) return;

    SpringArmComponent = Cast<USpringArmComponent>(pawn->GetComponentByClass(USpringArmComponent::StaticClass()));
    CameraComponent = Cast<UCameraComponent>(pawn->GetComponentByClass(UCameraComponent::StaticClass()));
    CapsuleComponent = Cast<UCapsuleComponent>(pawn->GetComponentByClass(UCapsuleComponent::StaticClass()));
}

void AOcclusionAwarePlayerController::SyncOccludedActors(){
    if(!ShouldCheckCameraOcclusion()) return;

    // If our camera is already colliding with something, its probably inside a wall or something
    // so just show all actors with no further tests
    if(SpringArmComponent->bDoCollisionTest){
        ForceShowOccludedActors();
        return;
    }

    // Raycast line
    FVector start = CameraComponent->GetComponentLocation();
    FVector end = GetPawn()->GetActorLocation();

    // Only check for static objects
    TArray<TEnumAsByte<EObjectTypeQuery>> collisionObjectTypes;
    
    // ECollisionChannel: https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Engine/ECollisionChannel/
    collisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic)); 
    collisionObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

    TArray<AActor*> asctorsToIgnore; // TODO: Add configuration to ignore actor types
    TArray<FHitResult> outHits;

    auto shouldDebug = DebugLineTraces ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;

    // Capsule shaped raycast?
    bool bGotHits = UKismetSystemLibrary::CapsuleTraceMultiForObjects(
        GetWorld(),
        start,
        end,
        CapsuleComponent->GetScaledCapsuleRadius() * CapsulePercentageForTrace,
        CapsuleComponent->GetScaledCapsuleHalfHeight() * CapsulePercentageForTrace,
        collisionObjectTypes,
        true, // bool bTraceComplex
        asctorsToIgnore,
        shouldDebug,
        outHits,
        true // bool bIgnoreSelf
    );

    // Actors that got occluded, hide them
    if(bGotHits){
        TSet<const AActor*> actorsJustOccluded;

        for(FHitResult hit : outHits){
            const AActor* hitActor = Cast<AActor>(hit.GetActor());
            HideOccludedActor(hitActor);
            actorsJustOccluded.Add(hitActor);
        }

        // Actors that are NOT occluded anymore, show them
        for(auto& item : OccludedActors){
            FOccludedActorProperties properties = item.Value;

            // Go through our list of already occluded actors and if any of them is not on the actorsJustOccluded 
            // and was being occluded, show them
            if(!actorsJustOccluded.Contains(properties.Actor) && properties.IsOccluded){
                ShowOccludedActor(properties);
                
                if(DebugLineTraces){
                    UE_LOG(LogTemp, Warning, 
                        TEXT("[%s.SyncOccludedActors()] Actor %s was occluded, but it's not occluded anymore with the new hits."), 
                        *this->GetName(),
                        *properties.Actor->GetName());
                }
            }
        }

    } else {
        // No actor inside raycast (no actor to occlude), show all actors in our map, if any
        ForceShowOccludedActors();
    }
}

bool AOcclusionAwarePlayerController::HideOccludedActor(const AActor* actor){
    FOccludedActorProperties *existingOccludedActor = OccludedActors.Find(actor);

    if(existingOccludedActor && existingOccludedActor->IsOccluded){
        if (DebugLineTraces) {
            UE_LOG(LogTemp, Warning, 
                TEXT("[%s.HideOccludedActor()] Actor %s was already occluded. Ignoring."), 
                *this->GetName(), 
                *actor->GetName());
        }
        return false;
    }


    if(existingOccludedActor && IsValid(existingOccludedActor->Actor)){
        existingOccludedActor->IsOccluded = true;
        OnHideOccludedActor(*existingOccludedActor);

        if (DebugLineTraces) {
            UE_LOG(LogTemp, Warning, 
                TEXT("[%s.HideOccludedActor()] Actor %s exists, but was not occluded. Occluding it now."), 
                *this->GetName(), 
                *actor->GetName());
        }

    } else {
        UStaticMeshComponent* staticMesh = Cast<UStaticMeshComponent>(actor->GetComponentByClass(UStaticMeshComponent::StaticClass()));

        FOccludedActorProperties occludedActor = {
            .Actor = actor,
            .StaticMesh = staticMesh,
            .Materials = staticMesh->GetMaterials(),
            .IsOccluded = true
        };

        OccludedActors.Add(actor, occludedActor);
        OnHideOccludedActor(occludedActor);
        
        if (DebugLineTraces) {
            UE_LOG(LogTemp, Warning, 
                TEXT("[%s.HideOccludedActor()] Actor %s does not exist, creating and occluding it now."), 
                *this->GetName(), 
                *actor->GetName());
        }
    }

    return true;
}

void AOcclusionAwarePlayerController::ShowOccludedActor(FOccludedActorProperties& occludedActor){
    if(!IsValid(occludedActor.Actor)){
        OccludedActors.Remove(occludedActor.Actor);
    }

    occludedActor.IsOccluded = false;
    OnShowOccludedActor(occludedActor);
}

bool AOcclusionAwarePlayerController::OnHideOccludedActor(const FOccludedActorProperties& occludedActor) const {
    for(int i = 0; i < occludedActor.StaticMesh->GetNumMaterials(); i++){
        occludedActor.StaticMesh->SetMaterial(i, TransparencyMaterial);
    }

    return true;
}

bool AOcclusionAwarePlayerController::OnShowOccludedActor(const FOccludedActorProperties& occludedActor) const {
    for(int materialIdx = 0; materialIdx < occludedActor.Materials.Num(); materialIdx++){
        occludedActor.StaticMesh->SetMaterial(materialIdx, occludedActor.Materials[materialIdx]);
    }

    return true;
}

void AOcclusionAwarePlayerController::ForceShowOccludedActors(){
    for(auto& item : OccludedActors){
        FOccludedActorProperties properties = item.Value;

        if(properties.IsOccluded){
            ShowOccludedActor(properties);

            if (DebugLineTraces) {
                UE_LOG(LogTemp, Warning, 
                    TEXT("[%s.ForceShowOccludedActors()] Actor %s was occluded, force to show again."), 
                    *this->GetName(),
                    *properties.Actor->GetName());
            }
        }
    }
}

