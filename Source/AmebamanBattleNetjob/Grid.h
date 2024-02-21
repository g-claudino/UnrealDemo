// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.h"
#include "Grid.generated.h"

UCLASS()
class AMEBAMANBATTLENETJOB_API AGrid : public AActor {
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGrid();

	// Conversions between grid location and world location
	UFUNCTION(BlueprintCallable)
	inline FIntVector WorldToGridLocation(FVector location);
	UFUNCTION(BlueprintCallable)
	FIntVector MoveInGrid(FIntVector currentLocation, FIntVector direction, int32 scale);

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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector Cells;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh *TileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGridTile> TileBlueprint;


private:
	TArray<AGridTile *> Grid;

	UFUNCTION(BlueprintCallable)
	void GenerateGrid(FIntVector dimensions);
	void GenerateGrid(int width, int depth, int height);


	// Conversions between [x, y, z] 3D array to [idx] 1D array of positions
	inline int FIntVectorToGridArrayIndex(int x, int y, int z);
	inline int FIntVectorToGridArrayIndex(FIntVector index3D);
	inline FIntVector GridArrayIndexToFIntVector(int idx);
};
