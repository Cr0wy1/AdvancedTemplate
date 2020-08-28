// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "AdvancedTemplate/Player/AdvancedCharacter.h"
#include "GameFramework/PlayerController.h"
//#include "Player/AdvancedPlayerController.h"
//#include "AdvancedGameInstance.h"
#include "Blueprint/UserWidget.h"

AActor* UAdvancedLibrary::SpawnActorFromTemplate(AActor* actorTemplate, const FTransform &transform, ESpawnActorCollisionHandlingMethod collisionHandling, AActor* owner) {

	if (actorTemplate) {
		FActorSpawnParameters spawnParams;
		spawnParams.Template = actorTemplate;
		spawnParams.SpawnCollisionHandlingOverride = collisionHandling;
		spawnParams.Owner = owner;
		
		return actorTemplate->GetWorld()->SpawnActor(actorTemplate->GetClass(), &transform, spawnParams);
	}
	return nullptr;
}

UWidget * UAdvancedLibrary::CreateAddViewport(APlayerController * ownerController, TSubclassOf<UUserWidget> widgetClass) {
	
	UUserWidget* createdWidget = CreateWidget(ownerController, widgetClass);
	createdWidget->AddToViewport();
	
	return createdWidget;
}

//UAdvancedGameInstance* UAdvancedLibrary::GetAdvancedGameInstance(const UObject* worldContextObject) {
	//return Cast<UAdvancedGameInstance>(UGameplayStatics::GetGameInstance(worldContextObject));
//}

AAdvancedCharacter* UAdvancedLibrary::GetAdvancedCharacter(const UObject* worldContextObject, int32 playerIndex) {
	return Cast<AAdvancedCharacter>(UGameplayStatics::GetPlayerCharacter(worldContextObject, 0));
}

//AAdvancedPlayerController * UAdvancedLibrary::GetAdvancedPlayerController(const UObject* worldContextObject, int32 playerIndex) {
	//return Cast<AAdvancedPlayerController>(UGameplayStatics::GetPlayerController(worldContextObject, 0));
//}
