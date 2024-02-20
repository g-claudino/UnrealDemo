// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPawn.h"
#include "EGridMovementDirection.h"

// Sets default values
AGridPawn::AGridPawn() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay() {
	Super::BeginPlay();
		UE_LOG(LogTemp, Error, TEXT("Grid is null for GridPawn! %p"), Grid);
	SetOnGrid(InitialGridPos);
}

void AGridPawn::SetOnGrid(FIntVector pos){
	if(Grid == nullptr){
		UE_LOG(LogTemp, Error, TEXT("Grid is null for GridPawn!"));
		return;
	}

	FVector location = Grid->GridToWorldLocation(pos);
	float halfHeight = this->GetSimpleCollisionHalfHeight();
	location.Z += halfHeight;

	UE_LOG(LogTemp, Display, TEXT("[GridPawn] Setting pawn position (%f, %f, %f)"), location.X, location.Y, location.Z);
	// UE_LOG(LogTemp, Display, TEXT("moving actor from (%d, %d) to (%d, %d)", 
	// 	pos.X, pos.Y,
	// 	location.X, location.Y));

	// static int i = 0;
	// GEngine->AddOnScreenDebugMessage(i++, 60.0, FColor::Cyan, "displayName: " + displayName);
	// GEngine->AddOnScreenDebugMessage(i++, 60.0, FColor::Cyan, "playerUUID: " + playerUUID);

	SetActorLocation(location);
}

void AGridPawn::MoveOnGrid(EGridMovementDirection direction, float actionValue){

}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


