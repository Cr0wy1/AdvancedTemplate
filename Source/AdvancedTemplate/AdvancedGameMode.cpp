// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedGameMode.h"
#include "Player/AdvancedCharacter.h"
#include "Player/AdvancedPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AAdvancedGameMode::AAdvancedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	DefaultPawnClass = AAdvancedCharacter::StaticClass();
	PlayerControllerClass = AAdvancedPlayerController::StaticClass();

	// use our custom HUD class
	//HUDClass = AAdvancedTemplateHUD::StaticClass();
}