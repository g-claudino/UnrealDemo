// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
// #include "World.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AMEBAMANBATTLENETJOB_API AMainMenuGameMode : public AGameModeBase {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category="MainMenu")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="MainMenu")
	TSubclassOf<UUserWidget> InitialMenuWidget;

	UPROPERTY()
	UUserWidget *CurrentWidget;

	// UPROPERTY()
	// World *InitialLevel;
};
