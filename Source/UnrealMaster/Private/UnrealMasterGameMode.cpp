// Copyright Epic Games, Inc. All Rights Reserved.

#include "../Public/UnrealMasterGameMode.h"
#include "../Public/UnrealMasterCharacter.h"
#include "UObject/ConstructorHelpers.h"

AUnrealMasterGameMode::AUnrealMasterGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
