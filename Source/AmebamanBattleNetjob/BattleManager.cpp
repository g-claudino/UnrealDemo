// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h" 
#include "BattleManager.h"

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

	Enemies.Init(nullptr, EnemyBlueprint.Num());
	for(int i = 0; i < EnemyBlueprint.Num(); i++){
		Enemies[i] = world->SpawnActor<AGridPawn>(EnemyBlueprint[i]);
		Enemies[i]->Setup(EnemyGrid, this);
		EnemyGrid->PlacePawnInGrid(Enemies[i], EnemiesGridInitialLocation[i]);

		// TODO: Create AI controller for this
		// auto controller = UGameplayStatics::GetSOMEAIController(world, 0);
		// controller->Possess(enemy);
	}
}

void ABattleManager::PlayerAttackCallback(FIntVector target){
	AttackOnGrid(EnemyGrid, target);
}

void ABattleManager::EnemyAttackCallback(FIntVector target){
	AttackOnGrid(PlayerGrid, target);
}

void ABattleManager::AttackOnGrid(AGrid* grid, FIntVector target){
	// TODO when setting a pawn on the grid, make the grid aware of the pawn so we can do collision 
	// checks for movement -- this means we can delegate pawn management to the grid instead of this
	// battle manager and we can differentiate between grids
	// for now this will always target enemy grid, just to test for damage, etc
	for(int i = 0; i < Enemies.Num(); i++){
		// TODO change this to query the grid for what is in this position

		// TODO: Create AI controller for this
		// auto controller = UGameplayStatics::GetSOMEAIController(world, 0);
		// controller->Possess(enemy);
	}
}

