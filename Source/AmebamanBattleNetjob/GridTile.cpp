// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"

AGridTile::AGridTile() : AGridTile::AGridTile(FVector(0.0, 0.0, 0.0), FVector(1.0, 1.0, 1.0), nullptr) {}

AGridTile::AGridTile(FVector worldLocation, FVector worldScale, UStaticMesh *mesh){
	PrimaryActorTick.bCanEverTick = false; // disable update until e actually need it

	SetActorLocation(worldLocation);
	SetActorScale3D(worldScale);

	StaticMesh = mesh;
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GridTileMesh"));
	SetRootComponent(StaticMeshComponent);
	// StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	SetMesh(StaticMesh);
}

// Called when the game starts or when spawned
void AGridTile::BeginPlay() {
	Super::BeginPlay();
}

void AGridTile::SetMesh(UStaticMesh *mesh){
	StaticMeshComponent->SetStaticMesh(mesh);
}


void AGridTile::PostEditChangeProperty(struct FPropertyChangedEvent& e) {
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(AGridTile, StaticMesh)){
        SetMesh(StaticMesh);
	}

    Super::PostEditChangeProperty(e);
}

