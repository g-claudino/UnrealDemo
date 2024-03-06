// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/CapsuleComponent.h" 
#include "Grid.h"
#include "BattleManager.h"
#include "GridPawn.h"

// Sets default values
AGridPawn::AGridPawn() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay() {
	Super::BeginPlay();
	CurrentHealth = MaxHP;
	CapsuleCollision = FindComponentByClass<UCapsuleComponent>();
}

void AGridPawn::Setup(AGrid *grid, ABattleManager *battleManager){
	Grid = grid;
	BattleManager = battleManager;
}


inline float AGridPawn::GetCollisionHalfHeight(){
	return CapsuleCollision->GetScaledCapsuleHalfHeight();
}


// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGridPawn::Attack(FIntVector target){
	BattleManager->PlayerAttackCallback(target, 1); // TODO refactor this hardcoded so the damage value originates from attack/skill dmg value
}


inline float AGridPawn::Smoothstep(float t){
	float t3 = t*t*t;
	float t4 = t3*t;
	float t5 = t4*t;
	return 6*t5 - 15*t4 + 10*t3;
}

void AGridPawn::Damage(int32 damage){
	CurrentHealth -= damage;
	if(CurrentHealth <= 0){
		OnKill();
	} else {
		OnDamage(damage, CurrentHealth);
	}
}


