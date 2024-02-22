// Fill out your copyright notice in the Description page of Project Settings.
#include "Grid.h"
#include "GridTile.h"
#include "CoreMinimal.h"

AGrid::AGrid(){
	PrimaryActorTick.bCanEverTick = false; // disable update until e actually need it
	if(TileBlueprint == nullptr) {
		TileBlueprint = AGridTile::StaticClass();
	}
}

// Called when the game starts or when spawned
void AGrid::BeginPlay() {
	Super::BeginPlay();
}

inline void AGrid::GenerateGrid(FIntVector dimensions){
	GenerateGrid(dimensions.X, dimensions.Y, dimensions.Z);
}

void AGrid::GenerateGrid(int width, int depth, int height) {
	ensureMsgf (width || depth || height <= 0, TEXT("Grid dimensions must be bigger than 0"));

	Cells.X = width;
	Cells.Y = depth;
	Cells.Z = height;

	int gridArraySize = Cells.X*Cells.Y*Cells.Z;
	if(Grid.Num() <= gridArraySize){
		Grid.SetNum(gridArraySize);
	}

	FVector location = GetActorLocation();
	FVector scale = GetActorScale();
	FRotator rotation = GetActorRotation();

	for(int x = 0; x < Cells.X; x++){
		for(int y = 0; y < Cells.Y; y++){
			for(int z = 0; z < Cells.Z; z++){

				location = FVector(Offset.X*x, Offset.Y*y, Offset.Z*z);

				FTransform const &tileTransform = { rotation, location, scale };
				FActorSpawnParameters params = {};
				params.Name = FName(FString::Printf(TEXT("Grid tile [%d, %d, %d]"), x, y, z));
				AGridTile *tileActor = GetWorld()->SpawnActor<AGridTile>(
					TileBlueprint,
					tileTransform,
					params
				);

				tileActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				tileActor->SetMesh(TileMesh);
				Grid[FIntVectorToGridArrayIndex(x, y, z)] = tileActor;
			}
		}
	}
}

// Conversions between grid location and world location
inline FIntVector AGrid::WorldToGridLocation(FVector location) {
	return {};
}

inline FVector AGrid::GridToWorldLocation(int x, int y, int z) {
	auto cell = Grid[FIntVectorToGridArrayIndex(x, y, z)];
	float halfHeight = cell->GetSimpleCollisionHalfHeight();
	FVector cellLocation = Grid[FIntVectorToGridArrayIndex(x, y, z)]->GetActorLocation();
	cellLocation.Y += halfHeight + this->GetSimpleCollisionHalfHeight();
	return cellLocation; 
}

inline FVector AGrid::GridToWorldLocation(FIntVector location) {
	return GridToWorldLocation(location.X, location.Y, location.Z);
}

inline bool AGrid::IsValidPosition(FIntVector location){
	return  location.X >= 0 && location.X < Cells.X &&
			location.Y >= 0 && location.Y < Cells.Y &&
			location.Z >= 0 && location.Z < Cells.Z;
}

FIntVector AGrid::MoveInGrid(FIntVector currentLocation, FIntVector direction, int32 scale){
	FIntVector target = currentLocation + direction * scale;
	if(IsValidPosition(target)){
		return target;
	} else {
		return currentLocation;
	}
}

inline FIntVector AGrid::GetMovementDirectionX(){ return FIntVector {1, 0, 0}; }
inline FIntVector AGrid::GetMovementDirectionY(){ return FIntVector {0, 1, 0}; }
inline FIntVector AGrid::GetMovementDirectionZ(){ return FIntVector {0, 0, 1}; }


// Conversions between [x, y, z] 3D array to [idx] 1D array of positions
inline int AGrid::FIntVectorToGridArrayIndex(int x, int y, int z) {
    return (z * Cells.X * Cells.Y) + (y * Cells.X) + x;
}

inline int AGrid::FIntVectorToGridArrayIndex(FIntVector index3D) {
    return (index3D.Z * Cells.X * Cells.Y) + (index3D.Y * Cells.X) + index3D.X;
}

inline FIntVector AGrid::GridArrayIndexToFIntVector(int idx) {
    int z = idx / (Cells.X * Cells.Y);
    idx -= (z * Cells.X * Cells.Y);
    int y = idx / Cells.X;
    int x = idx % Cells.X;
    return FIntVector(x, y, z);
}
