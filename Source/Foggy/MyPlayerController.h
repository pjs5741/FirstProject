// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foggy.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FOGGY_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	class UMyHUDWidget* GetHUDWidget() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
		TSubclassOf<class UMyHUDWidget> HUDWidgetClass;

private:

	UPROPERTY()
		class UMyHUDWidget* HUDWidget;
};
