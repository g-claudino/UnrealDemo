// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleManager.h"
#include "Kismet/GameplayStatics.h" 


// Sets default values
ABattleManager::ABattleManager(){
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ABattleManager::BeginPlay(){
	Super::BeginPlay();
}

void ABattleManager::SetupBattle(){
	auto world = GetWorld();
	const FTransform &playerTransform = {
		{0, 0, 0}, 
		{0, 0, 0}, 
		{1, 1, 1} 
	};
	const FTransform &enemyTransform = {
		{0, 0, 0}, 
		{0, 430, 0}, 
		{1, 1, 1} 
	};

	SpawnPlayer(world, playerTransform);
	SpawnEnemies(world, enemyTransform);
} 

void ABattleManager::SpawnPlayer(UWorld *world, const FTransform &transform){
	PlayerGrid = world->SpawnActor<AGrid>(PlayerGridBlueprint, transform);
	PlayerGrid->Offset = gridTilesOffset;
	PlayerGrid->GenerateGrid(PlayerGridDimensions);

	Player = world->SpawnActor<AGridPawn>(PlayerBlueprint);
	Player->Setup(PlayerGrid, this);
	PlayerGrid->PlacePawnInGrid(Player, PlayerGridInitialLocation);

	auto controller = UGameplayStatics::GetPlayerController(world, 0);
	controller->Possess(Player);
}
 
void ABattleManager::SpawnEnemies(UWorld *world, const FTransform &transform){
	EnemyGrid = world->SpawnActor<AGrid>(EnemyGridBlueprint, transform);
	EnemyGrid->Offset = gridTilesOffset;
	EnemyGrid->GenerateGrid(EnemyGridDimensions);

	for(int i = 0; i < EnemyBlueprint.Num(); i++){
		Enemies.Add(world->SpawnActor<AGridPawn>(EnemyBlueprint[i]));
		Enemies[i]->Setup(EnemyGrid, this);
		EnemyGrid->PlacePawnInGrid(Enemies[i], EnemiesGridInitialLocation[i]);

		// TODO: Create AI controller for this
		// auto controller = UGameplayStatics::GetSOMEAIController(world, 0);
		// controller->Possess(enemy);
	}
}



void ABattleManager::PlayerAttackCallback(FIntVector targetOffset, int damage){
	FTileData tileData;
	if (!PlayerGrid->GetPawnInfo(Player, tileData)) return;

	int playerLocationId = tileData.Id;
	FIntVector playerLocationVec = PlayerGrid->GridArrayIndexToFIntVector(playerLocationId);
	FIntVector target = playerLocationVec + targetOffset;
	ExecuteAttackOnGrid(EnemyGrid, tileData, target, damage);
}

void ABattleManager::EnemyAttackCallback(FIntVector targetOffset, int damage){
	FTileData tileData;
	ExecuteAttackOnGrid(PlayerGrid, tileData, targetOffset, damage);
}

void ABattleManager::ExecuteAttackOnGrid(AGrid* grid, const FTileData& tileData, FIntVector target, int damage){
	AGridPawn* targetPawn = tileData.Pawn;
	if(IsValid(targetPawn)){
		targetPawn->DamagePawn(damage);
	}
}


void ABattleManager::PlayerPreviewAttackDangerArea(FIntVector targetOffset){
	FTileData playerTileData;
	if (!PlayerGrid->GetPawnInfo(Player, playerTileData)) return;

	int playerLocationId = playerTileData.Id;
	FIntVector playerLocationVec = PlayerGrid->GridArrayIndexToFIntVector(playerLocationId);
	FIntVector target = playerLocationVec + targetOffset;

	FTileData enemyTileData; 
	if(EnemyGrid->GetPawnInfo(target, enemyTileData)){
		ExecutePreviewAttackDangerArea(EnemyGrid, enemyTileData, target);
	}
}

void ABattleManager::EnemyPreviewAttackDangerArea(const FIntVector targetOffset){
	const FTileData tileData = {};
	ExecutePreviewAttackDangerArea(PlayerGrid, tileData, targetOffset);
}

void ABattleManager::ExecutePreviewAttackDangerArea(AGrid* grid, const FTileData& tileData, FIntVector target){
	UStaticMeshComponent* staticMesh = Cast<UStaticMeshComponent>(tileData.Tile->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	// materialsTable.Add() staticMesh->GetMaterial()
	staticMesh->SetMaterial(0, DangerAreaHighlightMaterial);
}


void ABattleManager::RemovePawnFromGrid(AGridPawn* pawn){
	if(EnemyGrid->IsPawnInGrid(pawn)){
		if(EnemyGrid->RemovePawnFromGrid(pawn) <= 0){
			OnBattleWon();
		}
	} else {
		// No need to do anything with the grid here since we only have 1 player only, 
		// we just go to the game over flow
		OnBattleLost();
	}
}
