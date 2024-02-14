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
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int CellsX;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int CellsY;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int CellsZ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Offset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh *TileMesh;

private:	
	UFUNCTION(BlueprintCallable)
	void GenerateGrid(int width, int depth, int height);

	TArray<AGridTile*> Grid;
	inline int Vector3DPointToGrid(int x, int y, int z);
	inline FVector GridToPointVector3D(int idx);
};
