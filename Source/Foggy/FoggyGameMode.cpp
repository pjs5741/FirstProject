// Copyright Epic Games, Inc. All Rights Reserved.

#include "FoggyGameMode.h"
#include "FoggyCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFoggyGameMode::AFoggyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
