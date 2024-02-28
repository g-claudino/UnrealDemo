// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.h"
#include "GridPawn.h"
#include "Grid.generated.h"

// Should this be a class so UE can manage memory? does it handle structs with pointers properly? I think this will cause memory leaks
USTRUCT()
struct FTileData{
	GENERATED_BODY()

public:
	int32 Id;
	UPROPERTY()
	AGridPawn *Pawn;
	UPROPERTY()
	AGridTile *Tile;
};

UCLASS()
class AMEBAMANBATTLENETJOB_API AGrid : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

	UFUNCTION(BlueprintCallable)
	void GenerateGrid(FIntVector dimensions);
	void GenerateGrid(int width, int depth, int height);
	UFUNCTION(BlueprintCallable)
	void PlacePawnInGrid(AGridPawn *pawn, FIntVector location);

	// Conversions between grid location and world location
	UFUNCTION(BlueprintCallable)
	inline FIntVector WorldToGridLocation(FVector location);
	UFUNCTION(BlueprintCallable)
	bool CalculateTargetGridPosition(AGridPawn *pawn, FIntVector direction, int32 scale, FIntVector &result);
	bool CalculateTargetGridPosition(FIntVector currentLocation, FIntVector direction, int32 scale, FIntVector &result);
	UFUNCTION(BlueprintCallable)
	void MovePawnInGrid(AGridPawn *pawn, FIntVector newLocation);

	UFUNCTION(BlueprintCallable)
	inline FIntVector GetMovementDirectionX();
	UFUNCTION(BlueprintCallable)
	inline FIntVector GetMovementDirectionY();
	UFUNCTION(BlueprintCallable)
	inline FIntVector GetMovementDirectionZ();

	UFUNCTION(BlueprintCallable)
	inline FVector GridToWorldLocation(FIntVector location);
	inline FVector GridToWorldLocation(int x, int y, int z);

	UFUNCTION(BlueprintCallable)
	inline bool IsValidPosition(FIntVector location);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector Cells;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGridTile> TileBlueprint;


private:
	TArray<FTileData *> GridData;
	TMap<FString, FTileData *> GridPawnMap;

	// Conversions between [x, y, z] 3D array to [idx] 1D array of positions
	inline int FIntVectorToGridArrayIndex(int x, int y, int z);
	inline int FIntVectorToGridArrayIndex(FIntVector index3D);
	inline FIntVector GridArrayIndexToFIntVector(int idx);
};
