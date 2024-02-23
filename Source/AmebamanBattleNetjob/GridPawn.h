// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Grid.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridPawn.generated.h"


UCLASS()
class AMEBAMANBATTLENETJOB_API AGridPawn : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGridPawn();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector InitialGridPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntVector GridPos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGrid *Grid;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetOnGrid(FIntVector pos);

	UFUNCTION(BlueprintCallable)
	float Smoothstep(float t);
};
