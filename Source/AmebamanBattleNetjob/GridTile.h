// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridTile.generated.h"

UCLASS()
class AMEBAMANBATTLENETJOB_API AGridTile : public AActor {
	GENERATED_BODY()
	
public:
	AGridTile();
	AGridTile(FVector worldLocation, FVector worldScale, UStaticMesh *mesh);
	void SetMesh(UStaticMesh *mesh);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMesh *StaticMesh;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
	
private:
	UStaticMeshComponent *StaticMeshComponent;
};
