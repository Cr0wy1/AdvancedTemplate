// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairTraceComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UCrosshairTraceComponent::UCrosshairTraceComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCrosshairTraceComponent::BeginPlay() {
	Super::BeginPlay();

	playerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());

	if (bIgnoreOwner) {
		traceParams.AddIgnoredActor(GetOwner());
	}

}


// Called every frame
void UCrosshairTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CrosshairLineTrace(crosshairResult.hitResult, crosshairResult.hitDirection);

}


bool UCrosshairTraceComponent::CrosshairLineTrace(FHitResult &OUT_hitresult, FVector &OUT_Direction) {

	FVector startLocation, endLocation;
	int32 viewX, viewY;

	playerController->GetViewportSize(viewX, viewY);
	playerController->DeprojectScreenPositionToWorld(viewX * 0.5f, viewY * 0.5f, startLocation, OUT_Direction);

	endLocation = OUT_Direction * traceLength + startLocation;

	//ECC_GameTraceChannel4 = PlayerCrosshair
	GetWorld()->LineTraceSingleByChannel(OUT_hitresult, startLocation, endLocation, ECollisionChannel::ECC_GameTraceChannel4, traceParams);

	AActor* hittedActor = OUT_hitresult.GetActor();
	if (hittedActor) {
		if (latestHittedActor != hittedActor) {
			//UE_LOG(LogTemp, Warning, TEXT("Hit new ACtor"));

			OnHitNewActor.Broadcast(hittedActor);
			latestHittedActor = hittedActor;

		}
	}
	else {
		OUT_hitresult.Location = endLocation;
		OUT_hitresult.ImpactPoint = endLocation;
	}

	if (bDrawDebugPoint) {
		DrawDebugPoint(GetWorld(), OUT_hitresult.ImpactPoint, 10, FColor::Red);
		//DrawDebugSphere(GetWorld(), OUT_hitresult.ImpactPoint, 50, 10, FColor::Red);
	}

	return true;
}