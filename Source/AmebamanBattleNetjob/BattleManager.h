// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Grid.h"
#include "GridTile.h"
#include "GridPawn.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

// TODO make attack function a delegate to remove pawn dependency on the battle manager
// typedef void (ABattleManager::*AttackDelegate)(FIntVector target);


UCLASS()
class AMEBAMANBATTLENETJOB_API ABattleManager : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();
	void PlayerAttackCallback(FIntVector target_offset, int damage);
	void EnemyAttackCallback(FIntVector target_offset, int damage);
	void ExecuteAttackOnGrid(AGrid* grid, FIntVector target, int damage);
	UFUNCTION(BlueprintCallable)
	void RemovePawnFromGrid(AGridPawn* pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SetupBattle();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBattleWon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnBattleLost();
	UFUNCTION(BlueprintImplementableEvent)
	void OnAttackMiss();
	
	UPROPERTY(EditAnywhere)
	FVector GridTilesOffset = FVector{110, 110, 1100};
	UPROPERTY(EditAnywhere)
	FVector OffsetBetweenGrids = FVector{0,10,0};

	// Player properties
	UPROPERTY()
	AGrid *PlayerGrid;
	UPROPERTY()
	AGridPawn *Player;
	UPROPERTY(EditAnywhere, Category="Player")
	FIntVector PlayerGridInitialLocation;
	UPROPERTY(EditAnywhere, Category="Player")
	FIntVector PlayerGridDimensions;
	UPROPERTY(EditAnywhere, Category="Player")
	TSubclassOf<AGrid> PlayerGridBlueprint;
	UPROPERTY(EditAnywhere, Category="Player")
	TSubclassOf<AGridPawn> PlayerBlueprint;


	// Enemy properties
	UPROPERTY()
	AGrid *EnemyGrid;
	UPROPERTY()
	TArray<class AGridPawn *> Enemies;
	UPROPERTY(EditAnywhere, Category="Enemies")
	TArray<FIntVector> EnemiesGridInitialLocation;
	UPROPERTY(EditAnywhere, Category="Enemies")
	FIntVector EnemyGridDimensions;
	UPROPERTY(EditAnywhere, Category="Enemies")
	TSubclassOf<AGrid> EnemyGridBlueprint;
	UPROPERTY(EditAnywhere, Category="Enemies")
	TArray<TSubclassOf<AGridPawn>> EnemyBlueprint;

private:
	void SpawnPlayerGrid(UWorld *world, const FTransform &transform);
	void SpawnEnemyGrid(UWorld *world, const FTransform &transform);
	void SpawnPlayerActor(UWorld *world);
	void SpawnEnemiesActors(UWorld *world);
};
