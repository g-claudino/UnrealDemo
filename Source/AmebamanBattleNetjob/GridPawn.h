// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Grid.h"
#include "GridPawn.generated.h"


UCLASS()
class AMEBAMANBATTLENETJOB_API AGridPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGridPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    //Sets Character Movement Speed to Sprint values.
    void IA_Horizontal();

    //Sets Character Movement Speed back to default speed values.
    void IA_Vertical();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int GridX;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int GridY;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int GridZ;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AGrid *Grid;

public:	
	// Called every frame

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
