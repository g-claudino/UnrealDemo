// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.h"
#include "GridPawn.h"
#include "Grid.generated.h"

// Should this be a class so UE can manage memory? does it handle structs with pointers properly? I think this will cause memory leaks
USTRUCT()
struct FTileData {
	GENERATED_USTRUCT_BODY()

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
	FIntVector WorldToGridLocation(FVector location);
	UFUNCTION(BlueprintCallable)
	bool CalculateTargetGridPosition(const AGridPawn *pawn, FIntVector direction, int32 scale, FIntVector &result);
	bool CalculateTargetGridPosition(FIntVector currentLocation, FIntVector direction, int32 scale, FIntVector &result);
	UFUNCTION(BlueprintCallable)
	void MovePawnInGrid(AGridPawn *pawn, FIntVector newLocation);

	UFUNCTION(BlueprintCallable)
	inline FIntVector GetMovementDirectionX(){ return FIntVector {1, 0, 0}; }
	UFUNCTION(BlueprintCallable)
	inline FIntVector GetMovementDirectionY(){ return FIntVector {0, 1, 0}; }
	UFUNCTION(BlueprintCallable)
	inline FIntVector GetMovementDirectionZ(){ return FIntVector {0, 0, 1}; }

	UFUNCTION(BlueprintCallable)
	FVector GridToWorldLocation(FIntVector location);
	FVector GridToWorldLocation(int x, int y, int z);

	UFUNCTION()
	inline bool IsPawnInGrid(const AGridPawn *pawn) { return GridPawnMap.Find(pawn->GetName()) != nullptr; }
	UFUNCTION()
	bool GetPawnInfo(AGridPawn *pawn, FTileData& result);
	bool GetPawnInfo(FIntVector gridLocation, FTileData& result);
	bool GetPawnInfo(int32 gridIndex, FTileData& result);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset;

	// Conversions between [x, y, z] 3D array to [idx] 1D array of positions
	int FIntVectorToGridArrayIndex(int x, int y, int z);
	int FIntVectorToGridArrayIndex(FIntVector index3D);
	FIntVector GridArrayIndexToFIntVector(int idx);
	int32 RemovePawnFromGrid(AGridPawn *pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector Cells;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGridTile> TileBlueprint;

	UFUNCTION(BlueprintCallable)
	inline bool IsValidPosition(FIntVector location) { return IsLocationInBounds(location) && IsGridLocationEmpty(location); }
	UFUNCTION(BlueprintCallable)
	inline bool IsLocationInBounds(FIntVector location) {
		return  location.X >= 0 && location.X < Cells.X &&
				location.Y >= 0 && location.Y < Cells.Y &&
				location.Z >= 0 && location.Z < Cells.Z;
	}
	UFUNCTION(BlueprintCallable)
	inline bool IsIndexInBounds(int32 index) { return index < Cells.X*Cells.Y*Cells.Z; }
	UFUNCTION(BlueprintCallable)
	inline bool IsGridLocationEmpty(FIntVector location) { return GridData[FIntVectorToGridArrayIndex(location)].Pawn == nullptr; }

private:
	TArray<FTileData> GridData;
	TMap<FString, FTileData> GridPawnMap;
};
