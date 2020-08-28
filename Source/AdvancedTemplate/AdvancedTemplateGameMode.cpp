// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AdvancedTemplateGameMode.h"
#include "AdvancedTemplateHUD.h"
#include "AdvancedTemplateCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAdvancedTemplateGameMode::AAdvancedTemplateGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	//HUDClass = AAdvancedTemplateHUD::StaticClass();
}
