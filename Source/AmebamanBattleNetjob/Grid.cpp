// Fill out your copyright notice in the Description page of Project Settings.
#include "Grid.h"
#include "GridTile.h"
#include "CoreMinimal.h"

AGrid::AGrid(){
	PrimaryActorTick.bCanEverTick = false; // disable update until e actually need it
	if(TileBlueprint == nullptr) {
		TileBlueprint = AGridTile::StaticClass();
	}
	FVector scale = GetActorScale3D();
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
	if(GridData.Num() <= gridArraySize){
		GridData.SetNum(gridArraySize);
	}

	FVector location = GetActorLocation();
	FVector scale = GetActorScale();
	FRotator rotation = GetActorRotation();

	for(int x = 0; x < Cells.X; x++){
		for(int y = 0; y < Cells.Y; y++){
			for(int z = 0; z < Cells.Z; z++){
				location = FVector(Offset.X*x, Offset.Y*y, Offset.Z*z);
				FTransform const &tileTransform = { rotation, location, scale };

				// engine cannot handle same name for different objects, let it generate the name by itself
				// params.Name = FName(FString::Printf(TEXT("GridData tile [%d, %d, %d]"), x, y, z));
				AGridTile *tileActor = GetWorld()->SpawnActor<AGridTile>(TileBlueprint, tileTransform);
				tileActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);

				int gridIndex = FIntVectorToGridArrayIndex(x, y, z);
				GridData[gridIndex] = new FTileData();
				GridData[gridIndex]->Id = gridIndex;
				GridData[gridIndex]->Tile = tileActor;
				GridData[gridIndex]->Pawn = nullptr;
				// UE_LOG(
				// 	LogTemp, 
				// 	Display, 
				// 	TEXT("[%s.GenerateGrid()] GridData[%d,%d,%d]: { .Id: %d .Tile: %p, .Pawn: %p }"), 
				// 		*this->GetName(), 
				// 		x, y, z,
				// 		GridData[gridIndex]->Id,
				// 		GridData[gridIndex]->Tile,
				// 		GridData[gridIndex]->Pawn
				// );
			}
		}
	}
}

void AGrid::PlacePawnInGrid(AGridPawn *pawn, FIntVector gridPosition){
	ensureMsgf(IsValidPosition(gridPosition), TEXT("Trying to place pawn in invalid grid position"));
	
	int gridIndex = FIntVectorToGridArrayIndex(gridPosition);
	GridData[gridIndex]->Pawn = pawn;

	FVector location = GridToWorldLocation(gridPosition);
	float halfHeight = pawn->GetCollisionHalfHeight();
	location.Z += halfHeight;
	pawn->SetActorLocation(location);
	GridPawnMap.Add(pawn->GetName(), GridData[gridIndex]);
}


// Conversions between grid location and world location
inline FIntVector AGrid::WorldToGridLocation(FVector location) {
	return {};
}

inline FVector AGrid::GridToWorldLocation(int x, int y, int z) {
	int gridIndex = FIntVectorToGridArrayIndex(x, y, z);
	auto cell = GridData[gridIndex]->Tile;
	float halfHeight = cell->GetSimpleCollisionHalfHeight();
	FVector cellLocation = GridData[gridIndex]->Tile->GetActorLocation();
	cellLocation.Y += halfHeight + this->GetSimpleCollisionHalfHeight();
	return cellLocation; 
}

inline FVector AGrid::GridToWorldLocation(FIntVector location) {
	return GridToWorldLocation(location.X, location.Y, location.Z);
}

inline bool AGrid::IsValidPosition(FIntVector location){
	return IsLocationInBounds(location) && IsGridLocationEmpty(location);
}

inline bool AGrid::IsLocationInBounds(FIntVector location){
	return  location.X >= 0 && location.X < Cells.X &&
			location.Y >= 0 && location.Y < Cells.Y &&
			location.Z >= 0 && location.Z < Cells.Z;
}

inline bool AGrid::IsGridLocationEmpty(FIntVector location){
	return GridData[FIntVectorToGridArrayIndex(location)]->Pawn == nullptr;
}


bool AGrid::CalculateTargetGridPosition(AGridPawn *pawn, FIntVector direction, int32 scale, FIntVector &result){
	FIntVector currentLocation = GridArrayIndexToFIntVector(GridPawnMap[pawn->GetName()]->Id);
	return CalculateTargetGridPosition(currentLocation, direction, scale, result);
}

bool AGrid::CalculateTargetGridPosition(FIntVector currentLocation, FIntVector direction, int32 scale, FIntVector &result){
	FIntVector target = currentLocation + direction * scale;
	if(IsValidPosition(target)){
		result = target;
		return true;
	} else {
		return false;
	}
}


// This method doesn't physically move the pawn in the engine, just update it's current grid coordinates
void AGrid::MovePawnInGrid(AGridPawn *pawn, FIntVector newLocation){
	GridData[GridPawnMap[pawn->GetName()]->Id]->Pawn = nullptr;
	GridData[FIntVectorToGridArrayIndex(newLocation)]->Pawn = pawn;
	GridPawnMap[pawn->GetName()] = GridData[FIntVectorToGridArrayIndex(newLocation)];
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

inline bool AGrid::IsPawnInGrid(AGridPawn *pawn){
	return GridPawnMap.Find(pawn->GetName()) != nullptr;
}

bool AGrid::GetPawnInfo(AGridPawn *pawn, FTileData& result) { 
	FTileData* data = GridPawnMap[pawn->GetName()];
	if (data == nullptr){
		return false;
	}

	result = *data;
	return true;
}

bool AGrid::GetPawnInfo(FIntVector gridLocation, FTileData& result) {
	FTileData* data = GridData[FIntVectorToGridArrayIndex(gridLocation)];
	if (data == nullptr){
		return false;
	}

	result = *data;
	return true;
}

int32 AGrid::RemovePawnFromGrid(AGridPawn* pawn){
	FTileData tileData;
	if(GetPawnInfo(pawn, tileData)){
		GridPawnMap.Remove(pawn->GetName());
		GridData[tileData.Id]->Pawn = nullptr;
	}

	return GridPawnMap.Num();
}
