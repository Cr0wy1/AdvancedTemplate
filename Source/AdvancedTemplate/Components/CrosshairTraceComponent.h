// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CrosshairTraceComponent.generated.h"



//DECLARE_EVENT_OneParam(UCrosshairTraceComponent, FOnHitNewActor, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitNewActorSignature, AActor*, actor);




USTRUCT(BlueprintType)
struct ADVANCEDTEMPLATE_API FCrosshairResult {
	GENERATED_BODY()

		FHitResult hitResult;
	FVector hitDirection;

	bool IsActorHit() const { return hitResult.GetActor(); }

	template<typename OtherClass>
	bool IsClass(OtherClass checkClass) const {
		if (hitResult.GetActor()) {
			return hitResult.GetActor()->IsA(checkClass);
		}
		return false;
	}

	//Cast and retun the hitted actor, return nullptr if cast fails
	template<typename OtherClass>
	OtherClass* GetHitActor() const {
		return Cast<OtherClass>(hitResult.GetActor());
	}

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDTEMPLATE_API UCrosshairTraceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	//Delegates
	UPROPERTY(BlueprintAssignable)
	FOnHitNewActorSignature OnHitNewActor;

	FCrosshairResult crosshairResult;

	// Sets default values for this component's properties
	UCrosshairTraceComponent();

protected:

	//Store latest hitted Actor for OnHitNewActor delegate
	AActor * latestHittedActor = nullptr;

	APlayerController * playerController = nullptr;
	FCollisionQueryParams traceParams;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	bool bIgnoreOwner = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	float traceLength = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trace")
	bool bDrawDebugPoint = true;

	// Called when the game starts
	virtual void BeginPlay() override;

	bool CrosshairLineTrace(FHitResult &OUT_hitresult, FVector &OUT_Direction);

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "CrosshairTrace")
	FHitResult GetHitResult() const { return crosshairResult.hitResult; }
	
	UFUNCTION(BlueprintCallable, Category = "CrosshairTrace")
	FVector GetHitDirection() const { return crosshairResult.hitDirection; }

	const FCrosshairResult* GetCrosshairResultPtr() const { return &crosshairResult; }
};
