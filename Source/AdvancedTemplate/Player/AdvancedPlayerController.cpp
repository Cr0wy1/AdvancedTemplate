// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedPlayerController.h"

void AAdvancedPlayerController::BeginPlay() {
	Super::BeginPlay();
}

void AAdvancedPlayerController::PlayerTick(float DeltaTime){
	
	//UE_LOG(LogTemp, Warning, TEXT("PlayerTick"));
	
	if (recoilRemain != 0.0f && maxRecoil >= FMath::Abs(recoilBackRemain)) {
		UE_LOG(LogTemp, Warning, TEXT("recoilRemain: %f"), recoilRemain);
		recoilTickAmount = recoilAmount / (recoilTime + 0.001f);
		float addedPitch = FMath::Abs(recoilTickAmount * DeltaTime) > FMath::Abs(recoilRemain) ? recoilRemain : recoilTickAmount * DeltaTime;

		AddPitchInput(addedPitch);
		recoilRemain -= addedPitch;
		recoilBackRemain += addedPitch;

		if (recoilRemain == 0.0f) {
			recoilBackAmount = recoilBackRemain;
		}
	}
	else if (recoilBackRemain != 0.0f) {

		recoilTickAmount = recoilBackAmount / (recoilTime + 0.001f);
		float addedPitch = FMath::Abs(recoilTickAmount * DeltaTime) > FMath::Abs(recoilBackRemain) ? recoilBackRemain : recoilTickAmount * DeltaTime;
		AddPitchInput(-addedPitch);

		recoilBackRemain -= addedPitch;
	}

	/*
	if (recoilTickAmount != 0.0f) {
		float addedPitch;
		float recoilRemain = recoilAmount - recoilBackAmount;
		if (recoilRemain != 0.0f) {
			addedPitch = FMath::Abs(recoilTickAmount * DeltaTime) > FMath::Abs(recoilRemain) ? recoilRemain : recoilTickAmount * DeltaTime;
		}
		else if (recoilBackAmount != 0.0f) {

		}

	}
	*/

	Super::PlayerTick(DeltaTime);
}

void AAdvancedPlayerController::AddRecoil(float amount, float time){
	recoilAmount = amount - recoilBackRemain;
	recoilTime = time;
	recoilRemain = recoilAmount;

	//recoilTickAmount = recoilRemain;

	//if (recoilTime > 0.0f) {
		//recoilTickAmount = recoilAmount / (recoilTime + 0.001f);
	//}
	
	//AddPitchInput(amount);
}

//void AAdvancedPlayerController::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction & ThisTickFunction){
	//Super::TickActor(DeltaTime, TickType, ThisTickFunction);
	//UE_LOG(LogTemp, Warning, TEXT("Tick Actor"));
//}
