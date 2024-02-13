// Fill out your copyright notice in the Description page of Project Settings.
#include "Grid.h"
#include "GridTile.h"


AGrid::AGrid(){
	PrimaryActorTick.bCanEverTick = false; // disable update until e actually need it
}

// Called when the game starts or when spawned
void AGrid::BeginPlay() {
	Super::BeginPlay();
}

void AGrid::GenerateGrid(int width, int depth, int height) {
	checkf (width || depth || height <= 0, TEXT("Grid dimensions must be bigger than 0"));

	CellsX = width;
	CellsY = depth;
	CellsZ = height;

	int gridArraySize = CellsX*CellsY*CellsZ;
	if(Grid.Num() <= gridArraySize){
		Grid.SetNum(gridArraySize);
	}

	FVector location = GetActorLocation();
	FVector scale = GetActorScale();
	FRotator rotation = GetActorRotation();

	for(int x = 0; x < CellsX; x++){
		for(int y = 0; y < CellsY; y++){
			for(int z = 0; z < CellsZ; z++){
				
				location = FVector(Offset.X*x, Offset.Y*y, Offset.Z*z);

				FTransform const &tileTransform = { rotation, location, scale };
				FActorSpawnParameters params = {};
				params.Name = FName(FString::Printf(TEXT("Grid tile [%d, %d, %d]"), x, y, z));
				AGridTile *tileActor = GetWorld()->SpawnActor<AGridTile>(
					AGridTile::StaticClass(), 
					tileTransform, 
					params
				);

				tileActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				tileActor->SetMesh(TileMesh);
				Grid[Vector3DPointToGrid(x, y, z)] = tileActor;
			}
		}
	}
}

inline int AGrid::Vector3DPointToGrid(int x, int y, int z) {
    return (z * CellsX * CellsY) + (y * CellsX) + x;
}

inline FVector AGrid::GridToPointVector3D(int idx) {
    int z = idx / (CellsX * CellsY);
    idx -= (z * CellsX * CellsY);
    int y = idx / CellsX;
    int x = idx % CellsX;
    return FVector(x, y, z);
}
