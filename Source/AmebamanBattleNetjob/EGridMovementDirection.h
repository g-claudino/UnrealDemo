// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGridMovementDirection : uint8 {
    X UMETA(DisplayName="X axis"),
    Y UMETA(DisplayName="Y axis"),
    Z UMETA(DisplayName="Z axis"),
};

// #define GridDirectionX GGridMovementDirection.X;
// #define GridDirectionY GGridMovementDirection.Y;
// #define GridDirectionZ GGridMovementDirection.Z;

// struct {
//     const FIntVector X = FIntVector {1, 0, 0};
//     const FIntVector Y = FIntVector {0, 1, 0};
//     const FIntVector Z = FIntVector {0, 0, 1};
// } FGridMovementDirection;

// const struct FGridMovementDirection GGridMovementDirection;



