// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EGridMovementDirection : uint8 {
    X UMETA(DisplayName="X axis"),
    Y UMETA(DisplayName="Y axis"),
    Z UMETA(DisplayName="Z axis"),
};
