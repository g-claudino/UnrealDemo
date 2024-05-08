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

	inline float GetCollisionHalfHeight() { return CapsuleCollision->GetScaledCapsuleHalfHeight(); }

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
	void OnDamageTaken(const int32 damage, const int32 health);
	UFUNCTION(BlueprintImplementableEvent)
	void OnPawnKilled();

	UFUNCTION(BlueprintCallable)
	void Attack(const FIntVector targetOffset, int damage=1);

	UFUNCTION(BlueprintCallable)
	inline float Smoothstep(float t) {
		float t3 = t*t*t;
		float t4 = t3*t;
		float t5 = t4*t;
		return 6*t5 - 15*t4 + 10*t3;
	}
	
private:
	UPROPERTY()
	UCapsuleComponent *CapsuleCollision;

	UFUNCTION(BlueprintCallable)
	void PreviewAttackDangerArea(const FIntVector target);
};
