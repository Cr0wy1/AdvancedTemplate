// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AdvancedPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDTEMPLATE_API AAdvancedPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	float recoilAmount = 0.0f;
	float recoilRemain = 0.0f;

	float recoilBackAmount = 0.0f;
	float recoilBackRemain = 0.0f;

	float recoilTime = 0.0f;
	float recoilTickAmount = 0.0f;

	float maxRecoil = 3.0f;
	

	virtual void BeginPlay() override;

public:

	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Advanced")
	void AddRecoil(float amount, float time);

	//virtual void TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction & ThisTickFunction) override;
};
