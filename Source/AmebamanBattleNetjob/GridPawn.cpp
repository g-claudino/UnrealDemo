// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPawn.h"

// Sets default values
AGridPawn::AGridPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
}


// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGridPawn::IA_Horizontal()
{
	UE_LOG(LogTemp, Display, TEXT("Horizontal"));    
}

void AGridPawn::IA_Vertical()
{
    UE_LOG(LogTemp, Display, TEXT("Vertical"));
}


