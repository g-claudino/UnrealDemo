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
		{0, 0, 0}, 
		{1, 1, 1} 
	};

	SpawnPlayer(world, playerTransform);
	SpawnEnemies(world, enemyTransform);
} 

void ABattleManager::SpawnPlayer(UWorld *world, const FTransform &transform){
	PlayerGrid = world->SpawnActor<AGrid>(PlayerGridBlueprint, transform);
	PlayerGrid->Offset = GridTilesOffset;
	PlayerGrid->GenerateGrid(PlayerGridDimensions);

	Player = world->SpawnActor<AGridPawn>(PlayerBlueprint);
	Player->Setup(PlayerGrid, this);
	PlayerGrid->PlacePawnInGrid(Player, PlayerGridInitialLocation);

	auto controller = UGameplayStatics::GetPlayerController(world, 0);
	controller->Possess(Player);
}
 
void ABattleManager::SpawnEnemies(UWorld *world, const FTransform &transform){
	// Move enemy grid according to Player Grid Size and Center
	FVector PlayerGridSize = PlayerGrid->GetGridSize();
	FVector PlayerGridCenter = PlayerGrid->GetGridCenter();
	FVector PlayerGridOffset = {0, PlayerGridSize.Y+PlayerGridCenter.Y+GridOffset.Y, 0};

	// Generate Enemy Grid
	EnemyGrid = world->SpawnActor<AGrid>(EnemyGridBlueprint);
	EnemyGrid->Offset = GridTilesOffset;
	EnemyGrid->GenerateGrid(EnemyGridDimensions);
	FVector EnemyGridSize = EnemyGrid->GetGridSize();
	FVector EnemyGridCenter = EnemyGrid->GetGridCenter();
	FVector TotalOffset = {0,PlayerGridOffset.Y+EnemyGridSize.Y,0};
	
	// Update Transform
	FTransform RelocateTransform = transform;
	RelocateTransform.SetTranslation(TotalOffset);
	EnemyGrid->SetActorTransform(RelocateTransform);

	for(int i = 0; i < EnemyBlueprint.Num(); i++){
		Enemies.Add(world->SpawnActor<AGridPawn>(EnemyBlueprint[i]));
		Enemies[i]->Setup(EnemyGrid, this);
		EnemyGrid->PlacePawnInGrid(Enemies[i], EnemiesGridInitialLocation[i]);

		// TODO: Create AI controller for this
		// auto controller = UGameplayStatics::GetSOMEAIController(world, 0);
		// controller->Possess(enemy);
	}
}



void ABattleManager::PlayerAttackCallback(FIntVector target_offset, int damage){
	FTileData tileData;
	if (PlayerGrid->GetPawnInfo(Player, tileData)){
		int playerLocationID = tileData.Id;
		FIntVector playerLocationVec = PlayerGrid->GridArrayIndexToFIntVector(playerLocationID);
		FIntVector target = playerLocationVec+target_offset;
		ExecuteAttackOnGrid(EnemyGrid, target, damage);
	}
}

void ABattleManager::EnemyAttackCallback(FIntVector target_offset, int damage){
	ExecuteAttackOnGrid(PlayerGrid, target_offset, damage);
}

void ABattleManager::ExecuteAttackOnGrid(AGrid* grid, FIntVector target, int damage){
	FTileData gridData;
	if (grid->GetPawnInfo(target, gridData)){
		AGridPawn* gridPawnInLocation = gridData.Pawn;
		if (IsValid(gridPawnInLocation)){
			gridPawnInLocation->DamagePawn(damage);
		}
	}
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
