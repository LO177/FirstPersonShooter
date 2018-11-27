// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "TestProjectGameMode.h"
#include "TestProjectHUD.h"
#include "TestProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATestProjectGameMode::ATestProjectGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/ThePusher"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ATestProjectHUD::StaticClass();
}
