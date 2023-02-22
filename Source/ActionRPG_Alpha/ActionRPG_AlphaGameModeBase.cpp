// Copyright Epic Games, Inc. All Rights Reserved.


#include "ActionRPG_AlphaGameModeBase.h"

AActionRPG_AlphaGameModeBase::AActionRPG_AlphaGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawn(TEXT("Class'/Script/ActionRPG_Alpha.PlayerableCharacter'"));
	if (PlayerPawn.Class != NULL)
	{
		DefaultPawnClass = PlayerPawn.Class;
	}
}
