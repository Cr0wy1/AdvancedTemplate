// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReloadSignature, float, reloadTime);



class AProjectile;
class UCrosshairTraceComponent;



UENUM()
enum class EShootingType : uint8 {
	SINGLE,
	SALVE,
	RAPID,
	CHARGE
};


USTRUCT(BlueprintType)
struct ADVANCEDTEMPLATE_API FWeaponAmmo {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int32 magazineCapacity = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		int32 totalCapacity = 100;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		int32 currentMagazine = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Weapon")
		int32 currentTotal = 0;

	void Init() {
		currentMagazine = magazineCapacity;
		currentTotal = totalCapacity;
	}

	int32 RemoveMagazineAmmo(int32 amount) {
		int32 realRemoved = amount;
		currentMagazine -= amount;
		if (currentMagazine < 0) { 
			realRemoved = amount + currentMagazine;
			currentMagazine = 0;
		}
		return realRemoved;
	}

	int32 RemoveTotalAmmo(int32 amount) {
		int32 realRemoved = amount;
		currentTotal -= amount;
		if (currentTotal < 0) { 
			realRemoved = amount + currentTotal;
			currentTotal = 0; 
		}
		return realRemoved;
	}

	void Reload() {
		int32 leftCapacity = magazineCapacity - currentMagazine;
		currentMagazine += RemoveTotalAmmo(leftCapacity);
	}

	bool IsMagazineFull() { return currentMagazine == magazineCapacity; }
	bool IsTotalFull() { return currentTotal == totalCapacity; }
};

USTRUCT(BlueprintType)
struct ADVANCEDTEMPLATE_API FWeaponProperties {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		EShootingType shootingType = EShootingType::RAPID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float shotDelay = 0.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FWeaponAmmo ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float reloadDelay = 1.0f;

	//hold min Time Trigger before Weapon can shoot, 0 is without delay, used for charge Weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float minShootingTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float chargeSpeed = 1.0f;

	//1.0f = 100% accuracy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		float accuracy = 1.0f;
};



UCLASS()
class ADVANCEDTEMPLATE_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeapon();

protected:

	FActorSpawnParameters projectileSpawnParams;
	AProjectile* projectileTemplate;

	float delayEndTime = 0.0f;

	float startShootingTime = 0.0f;
	float stopShootingTime = 0.0f;
	float cShootingDuration = 0.0f;

	bool bIsShooting = false;
	bool bIsReloading = false;
	UCrosshairTraceComponent* crossTraceComp = nullptr;
	
	FTimerHandle relaodTimer;

	UPROPERTY(BlueprintAssignable)
	FOnReloadSignature OnReloadEvent;

	//COMPONENTS
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
		USkeletalMeshComponent* skeletalMeshComp;

	//PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		TSubclassOf<AProjectile> projectileBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
		FWeaponProperties properties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USoundBase* shotSound;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void BeforeProjectileFired(AProjectile* templateProjectile);
	virtual void AfterProjectileFired(AProjectile* firedProjectile);

	//Blueprint Events
	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
		void OnStartShooting();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
		void OnStopShooting();

	UFUNCTION(BlueprintImplementableEvent, Category = "Weapon")
		void OnFire(AProjectile* firedProjectile);


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Fire(const FVector targetLoc); 

	virtual void StartShooting();
	virtual void StopShooting();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual void Reload();

	UFUNCTION()
	void FinishReload();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void PressFireTrigger();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ReleaseFireTrigger();

	bool IsWeaponValid();

	void SetFocus(const FVector &targetLoc);
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
		void SetCrosshairAsTarget(UCrosshairTraceComponent* _crossTraceComp) { crossTraceComp = _crossTraceComp; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FORCEINLINE bool IsReloading() const { return bIsReloading; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FORCEINLINE bool IsShooting() const { return bIsShooting; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FORCEINLINE float GetCurrentShootingDuration() const { return cShootingDuration; }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FORCEINLINE float GetChargePercent() const { return FMath::Clamp(cShootingDuration / properties.chargeSpeed, 0.0f, 1.0f); }

	UFUNCTION(BlueprintCallable, Category = "Weapon")
		FORCEINLINE AProjectile* GetProjectileTemplate() const { return projectileTemplate; }

};
