// Fill out your copyright notice in the Description page of Project Settings.

// this is based on: https://alfredbaudisch.com/blog/gamedev/unreal-engine-ue/unreal-engine-actors-transparent-block-camera-occlusion-see-through/
// all credits to Alfred Reinold Baudisch (https://github.com/alfredbaudisch)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "OcclusionAwarePlayerController.generated.h"

USTRUCT(BlueprintType)
struct FOccludedActorProperties {
	GENERATED_USTRUCT_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	const AActor* Actor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool IsOccluded;
};

/**
 * 
 */
UCLASS()
class AMEBAMANBATTLENETJOB_API AOcclusionAwarePlayerController : public APlayerController {
	GENERATED_BODY()
	
public:
	AOcclusionAwarePlayerController();

	UFUNCTION(BlueprintCallable)
	void SyncOccludedActors();

protected:
	virtual void BeginPlay() override;

	/** How much of the Pawn capsule Radius and Height
	 * should be used for the Line Trace before considering an Actor occluded?
	 * Values too low may make the camera clip through walls.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion", meta=(ClampMin="0.1", ClampMax="10"))
	float CapsulePercentageForTrace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Materials")
	UMaterialInterface* TransparencyMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Components")
	class USpringArmComponent* SpringArmComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Components")
	class UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Components")
	class UCapsuleComponent* CapsuleComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion")
	bool IsOcclusionEnabled;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera Occlusion|Occlusion")
	bool DebugLineTraces;

private:
	TMap<const AActor*, FOccludedActorProperties> OccludedActors;

	bool HideOccludedActor(const AActor* actor);
	void ShowOccludedActor(FOccludedActorProperties& occludedActor);
	bool OnHideOccludedActor(const FOccludedActorProperties& occludedActor) const;
	bool OnShowOccludedActor(const FOccludedActorProperties& occludedActor) const;
	void ForceShowOccludedActors();
	
	__forceinline bool ShouldCheckCameraOcclusion() const {
		return IsOcclusionEnabled && TransparencyMaterial && CameraComponent && CapsuleComponent;
	}
};
