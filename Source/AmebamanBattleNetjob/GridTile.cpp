// Fill out your copyright notice in the Description page of Project Settings.

#include "GridTile.h"
#include "Engine/EngineTypes.h" 

AGridTile::AGridTile() {
	PrimaryActorTick.bCanEverTick = false; // disable update until e actually need it
	UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if(IsValid(mesh)){
		mesh->SetMobility(EComponentMobility::Stationary);
	}
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay() {
	Super::BeginPlay();
}

