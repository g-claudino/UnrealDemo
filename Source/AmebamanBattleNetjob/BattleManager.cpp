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
		{0, 0, 0},
		{1, 1, 1} 
	};

	SpawnPlayerGrid(world, playerTransform);
	SpawnPlayerActor(world);
	SpawnEnemyGrid(world, enemyTransform);
	SpawnEnemiesActors(world);
} 

void ABattleManager::SpawnPlayerGrid(UWorld *world, const FTransform &transform){
	PlayerGrid = world->SpawnActor<AGrid>(PlayerGridBlueprint, transform);
	PlayerGrid->Offset = GridTilesOffset;
	PlayerGrid->GenerateGrid(PlayerGridDimensions);
}

void ABattleManager::SpawnPlayerActor(UWorld *world){
	Player = world->SpawnActor<AGridPawn>(PlayerBlueprint);
	Player->Setup(PlayerGrid, this);
	PlayerGrid->PlacePawnInGrid(Player, PlayerGridInitialLocation);

	auto controller = UGameplayStatics::GetPlayerController(world, 0);
	controller->Possess(Player);
}
 
void ABattleManager::SpawnEnemyGrid(UWorld *world, const FTransform &transform){
	// Move enemy grid according to Player Grid Size and Center
	FVector PlayerGridSize = PlayerGrid->GetGridSize();
	FVector PlayerGridReferenceSize = {0, PlayerGridSize.Y+OffsetBetweenGrids.Y, 0};

	// Generate Enemy Grid
	EnemyGrid = world->SpawnActor<AGrid>(EnemyGridBlueprint);
	EnemyGrid->Offset = GridTilesOffset;
	EnemyGrid->GenerateGrid(EnemyGridDimensions);
	FVector EnemyGridSize = EnemyGrid->GetGridSize();
	FVector EnemyGridLocation = {0, PlayerGridReferenceSize.Y+EnemyGridSize.Y, 0};

	// Update Transform
	FTransform RelocateTransform = transform;
	RelocateTransform.SetTranslation(EnemyGridLocation);
	EnemyGrid->SetActorTransform(RelocateTransform);
}

void ABattleManager::SpawnEnemiesActors(UWorld *world){
	for(int i = 0; i < EnemyBlueprint.Num(); i++){
		Enemies.Add(world->SpawnActor<AGridPawn>(EnemyBlueprint[i]));
		Enemies[i]->Setup(EnemyGrid, this);
		EnemyGrid->PlacePawnInGrid(Enemies[i], EnemiesGridInitialLocation[i]);
	}
}


void ABattleManager::AttackFromGrid(AGrid* sourceGrid, FIntVector targetOffset, int damage){
	FTileData tileData;
	AGrid* targetGrid = EnemyGrid;

	// invert target grid
	if(sourceGrid == EnemyGrid){
		targetGrid = PlayerGrid;
	}

	if (targetGrid->GetPawnInfo(targetOffset, tileData)){
		AGridPawn* gridPawnInLocation = tileData.Pawn;
		if (IsValid(gridPawnInLocation)){
			gridPawnInLocation->DamagePawn(damage);
		} 
	} else {
		OnAttackMiss();
	}
}

void ABattleManager::PreviewAttackDangerArea(AGrid* sourceGrid, FIntVector target){
	FTileData tileData;
	AGrid* targetGrid = EnemyGrid;

	// invert target grid
	if(sourceGrid == EnemyGrid){
		targetGrid = PlayerGrid;
	}

	if(!targetGrid->GetPawnInfo(target, tileData)) {
		return;
	}

	SyncDangerAreaHighlights();

	UStaticMeshComponent* staticMesh = Cast<UStaticMeshComponent>(tileData.Tile->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	FHighlightActorProperties properties = {
		.Actor = tileData.Tile,
		.StaticMesh = staticMesh,
		.Materials = staticMesh->GetMaterials(),
		.IsHighlighted = true,
	};

	UMaterialInstanceDynamic* currentMaterial = staticMesh->CreateDynamicMaterialInstance(0);
	currentMaterial->SetScalarParameterValue("FresnelBlend", 1.0);

	HighlightedActors.Add(tileData.Tile, properties);
	staticMesh->SetMaterial(0, currentMaterial);
}

void ABattleManager::SyncDangerAreaHighlights(){
	for (auto& actor : HighlightedActors){
		FHighlightActorProperties& tileData = actor.Value;

		for(int materialIdx = 0; materialIdx < tileData.Materials.Num(); materialIdx++){
			tileData.StaticMesh->SetMaterial(materialIdx, tileData.Materials[materialIdx]);
		}
	}
	HighlightedActors.Reset();
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
