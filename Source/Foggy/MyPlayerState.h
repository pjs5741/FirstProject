// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foggy.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

/**
 * 
 */
UCLASS()
class FOGGY_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();
	int32 GetCharacterLevel() const;

	void InitPlayerData();

	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:

	UPROPERTY(Transient)
		int32 CharacterLevel;
};
