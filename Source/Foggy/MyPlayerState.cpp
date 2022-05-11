// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "MyGameInstance.h"

AMyPlayerState::AMyPlayerState()
{
	CharacterLevel = 1;
}



int32 AMyPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

void AMyPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("XXS"));
	CharacterLevel = 1;
}