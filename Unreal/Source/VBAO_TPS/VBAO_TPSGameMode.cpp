// Copyright Epic Games, Inc. All Rights Reserved.

#include "VBAO_TPSGameMode.h"
#include "VBAO_TPSCharacter.h"
#include "UObject/ConstructorHelpers.h"

AVBAO_TPSGameMode::AVBAO_TPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
