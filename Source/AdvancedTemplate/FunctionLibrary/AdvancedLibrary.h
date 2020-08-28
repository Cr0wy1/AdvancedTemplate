// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/EngineTypes.h"
#include "AdvancedLibrary.generated.h"




/**
 * 
 */
UCLASS()
class ADVANCEDTEMPLATE_API UAdvancedLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AdvancedLibrary")
	static AActor* SpawnActorFromTemplate( AActor* actorTemplate, const FTransform &transform, ESpawnActorCollisionHandlingMethod collisionHandling = ESpawnActorCollisionHandlingMethod::Undefined, AActor* owner = nullptr);


	UFUNCTION(BlueprintCallable, Category = "AdvancedLibrary", meta = (DeterminesOutputType = "widgetClass"))
		static class UWidget* CreateAddViewport(class APlayerController* ownerController, TSubclassOf<class UUserWidget> widgetClass);

	//UFUNCTION(BlueprintPure, Category = "AdvancedLibrary", meta = (WorldContext = "worldContextObject", UnsafeDuringActorConstruction = "true"))
		//static UAdvancedGameInstance* GetAdvancedGameInstance(const UObject* worldContextObject);

	UFUNCTION(BlueprintPure, Category = "AdvancedLibrary", meta = (WorldContext = "worldContextObject", UnsafeDuringActorConstruction = "true"))
		static class AAdvancedCharacter* GetAdvancedCharacter(const UObject* worldContextObject, int32 playerIndex = 0);

	//UFUNCTION(BlueprintPure, Category = "AdvancedLibrary", meta = (WorldContext = "worldContextObject", UnsafeDuringActorConstruction = "true"))
		//static AAdvancedPlayerController* GetAdvancedPlayerController(const UObject* worldContextObject, int32 playerIndex = 0);
};
