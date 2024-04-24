// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPawn.h"
#include "Grid.h"
#include "BattleManager.h"
#include "Components/CapsuleComponent.h" 

// Sets default values
AGridPawn::AGridPawn() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay() {
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	CapsuleCollision = FindComponentByClass<UCapsuleComponent>();
}

void AGridPawn::Setup(AGrid *grid, ABattleManager *battleManager){
	Grid = grid;
	BattleManager = battleManager;
}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGridPawn::Attack(const FIntVector targetOffset, int damage){
	FTileData tileData;

	if (Grid->GetPawnInfo(this, tileData)){
		int pawnLocationID = tileData.Id;
		FIntVector playerLocationVec = Grid->GridArrayIndexToFIntVector(pawnLocationID);
		FIntVector target = playerLocationVec+targetOffset;
		BattleManager->AttackFromGrid(Grid, target, damage); 
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s.Attack()] Couldn't find pawn data in grid %s"), *GetName(), *Grid->GetName());
}

void AGridPawn::DamagePawn(int32 damage){
	CurrentHealth -= damage;
	if(CurrentHealth <= 0){
		OnPawnKilled();
	} else {
		OnDamageTaken(damage, CurrentHealth);
	}
}

void AGridPawn::PreviewAttackDangerArea(const FIntVector targetOffset){
		FTileData tileData;

	if (Grid->GetPawnInfo(this, tileData)){
		int pawnLocationID = tileData.Id;
		FIntVector playerLocationVec = Grid->GridArrayIndexToFIntVector(pawnLocationID);
		FIntVector target = playerLocationVec+targetOffset;
		BattleManager->PreviewAttackDangerArea(Grid, target); 
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s.Attack()] Couldn't find pawn data in grid %s"), *GetName(), *Grid->GetName());

}

