// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h" 

#include "GridPawn.generated.h"

class AGrid;
class ABattleManager;

UCLASS()
class AMEBAMANBATTLENETJOB_API AGridPawn : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AGridPawn();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void Setup(AGrid *grid, ABattleManager *battleManager);

	float GetCollisionHalfHeight();

	UFUNCTION(BlueprintCallable)
	void DamagePawn(int32 damage);

protected:
	UPROPERTY(BluePrintReadOnly)
	int32 CurrentHealth;
	UPROPERTY(EditDefaultsOnly, BluePrintReadOnly)
	int32 MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	AGrid *Grid;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ABattleManager *BattleManager;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UFUNCTION(BlueprintImplementableEvent)
	void OnDamageTaken(int32 damage, int32 health);
	UFUNCTION(BlueprintImplementableEvent)
	void OnPawnKilled();

	UFUNCTION(BlueprintCallable)
	void Attack(FIntVector target);

	UFUNCTION(BlueprintCallable)
	float Smoothstep(float t);
	
private:
	UPROPERTY()
	UCapsuleComponent *CapsuleCollision;
};
