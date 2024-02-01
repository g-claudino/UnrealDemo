// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuGameMode.h"

void AMainMenuGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass){

	UE_LOG(LogTemp, Error, TEXT("[ChangeMenuWidget] Starting change widget"), *this->GetName());
	if(CurrentWidget != nullptr){
		UE_LOG(LogTemp, Error, TEXT("[ChangeMenuWidget] Cleaning viewport"), *this->GetName());
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}

	if(NewWidgetClass == nullptr) return;

	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
	UE_LOG(LogTemp, Error, TEXT("[ChangeMenuWidget] Creating new widget"), *this->GetName());
	if(CurrentWidget != nullptr){
	UE_LOG(LogTemp, Error, TEXT("[ChangeMenuWidget] Adding new widget to viewport"), *this->GetName());
		CurrentWidget->AddToViewport(0);
	} else {
		UE_LOG(LogTemp, Error, TEXT("Failed to create main menu widget"), *this->GetName());
	}
	UE_LOG(LogTemp, Error, TEXT("[ChangeMenuWidget]"), *this->GetName());
}

void AMainMenuGameMode::BeginPlay(){
	Super::BeginPlay();
	CurrentWidget = nullptr;
	ChangeMenuWidget(InitialMenuWidget);
}
