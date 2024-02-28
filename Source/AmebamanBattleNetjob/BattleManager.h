// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Grid.h"
#include "GridTile.h"
#include "GridPawn.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleManager.generated.h"

// TODO
// typedef void (ABattleManager::*AttackDelegate)(FIntVector target);


UCLASS()
class AMEBAMANBATTLENETJOB_API ABattleManager : public AActor {
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABattleManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void SetupBattle();
	
	UPROPERTY(EditAnywhere)
	FVector gridTilesOffset = FVector{110, 110, 1100};


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
	void SpawnPlayer(UWorld *world, const FTransform &transform);
	void SpawnEnemies(UWorld *world, const FTransform &transform);

public:
	void PlayerAttackCallback(FIntVector target);
	void EnemyAttackCallback(FIntVector target);
	void AttackOnGrid(AGrid* grid, FIntVector target);
};
