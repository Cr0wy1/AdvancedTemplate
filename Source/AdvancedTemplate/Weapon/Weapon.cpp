// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Projectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "AdvancedTemplate/Components/CrosshairTraceComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h" //for finsihActorSpawning
#include "Engine/Public/TimerManager.h"

// Sets default values
AWeapon::AWeapon() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	skeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SetRootComponent(skeletalMeshComp);



}

// Called when the game starts or when spawned
void AWeapon::BeginPlay() {
	Super::BeginPlay();

	properties.ammo.Init();

	projectileSpawnParams.Owner = this;
	projectileTemplate = GetWorld()->SpawnActorDeferred<AProjectile>(projectileBP, FTransform());

}

// Called every frame
void AWeapon::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	
	if (bIsShooting && crossTraceComp) {
		cShootingDuration = GetWorld()->GetTimeSeconds() - startShootingTime;

		if (properties.shootingType != EShootingType::CHARGE) {
			Fire(crossTraceComp->GetCrosshairResultPtr()->hitResult.ImpactPoint);
		}

		//TODO Maybe refactor this block
		if (properties.shootingType == EShootingType::SINGLE) {
			StopShooting();
		}
	}
}

void AWeapon::Fire(const FVector targetLoc) {
	//check if is reloading
	if (bIsReloading) return;

	//check ammo
	if (properties.ammo.currentMagazine <= 0) return;

	//check shot delay time
	if (GetWorld()->GetTimeSeconds() < delayEndTime) return;

	if (projectileBP) {

		FTransform projectileSpawnTrans;

		if (skeletalMeshComp->DoesSocketExist("Muzzle")) {
			projectileSpawnTrans = skeletalMeshComp->GetSocketTransform("Muzzle");
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Socket 'projectile' not found on Weapon"));
			projectileSpawnTrans.SetLocation( GetActorLocation() );
			projectileSpawnTrans.SetRotation(FQuat(GetActorRotation()) );
		}

		FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(projectileSpawnTrans.GetLocation(), targetLoc);

		//Random Rot for Accuracy
		float accuracyRange = cShootingDuration*90.0f - (cShootingDuration*90.0f * properties.accuracy);
		FVector2D randPoint = FMath::RandPointInCircle(accuracyRange);
		lookAtRot.Yaw += randPoint.X;
		lookAtRot.Pitch += randPoint.Y;

		projectileSpawnTrans.SetRotation(FQuat(lookAtRot));


		BeforeProjectileFired(projectileTemplate);

		projectileSpawnParams.Template = projectileTemplate;
		AProjectile* spawnedProjectile = GetWorld()->SpawnActor<AProjectile>(projectileBP, projectileSpawnTrans, projectileSpawnParams);
		
		properties.ammo.RemoveMagazineAmmo(1);

		AfterProjectileFired(spawnedProjectile);

		OnFire(spawnedProjectile);

		//Play Sound
		if (shotSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), shotSound, projectileSpawnTrans.GetLocation(), 0.2f);
		}
		

		//Set shot time
		delayEndTime = GetWorld()->GetTimeSeconds() + properties.shotDelay;
	}

}

void AWeapon::BeforeProjectileFired(AProjectile * templateProjectile) {
	
}

void AWeapon::AfterProjectileFired(AProjectile * firedProjectile) {

}

void AWeapon::StartShooting() {
	startShootingTime = GetWorld()->GetTimeSeconds();
	bIsShooting = true;

	OnStartShooting();
}

void AWeapon::StopShooting() {
	if (bIsShooting) {
		if (crossTraceComp && properties.shootingType == EShootingType::CHARGE &&  properties.minShootingTime < cShootingDuration) {
			Fire(crossTraceComp->GetCrosshairResultPtr()->hitResult.ImpactPoint);
		}
		//UE_LOG(LogTemp, Warning, TEXT("ShootingDuration: %f"), cShootingDuration);

		cShootingDuration = 0.0f;
		stopShootingTime = GetWorld()->GetTimeSeconds();
		bIsShooting = false;

		OnStopShooting();
	}
}

void AWeapon::Reload(){
	if (!bIsReloading && !properties.ammo.IsMagazineFull() && properties.ammo.currentTotal > 0) {
		bIsReloading = true;
		GetWorldTimerManager().SetTimer(relaodTimer, this, &AWeapon::FinishReload, properties.reloadDelay);
		OnReloadEvent.Broadcast(properties.reloadDelay);
	}
	
}

void AWeapon::FinishReload(){
	properties.ammo.Reload();
	bIsReloading = false;
}

void AWeapon::PressFireTrigger(){
	StartShooting();
}


void AWeapon::ReleaseFireTrigger(){
	StopShooting();
}

bool AWeapon::IsWeaponValid(){

	bool bReturnValue = true;

	if (!skeletalMeshComp->DoesSocketExist("Muzzle")) {
		UE_LOG(LogTemp, Warning, TEXT("%s: 'Muzzle' Socket on SkeletalMesh missed"), *GetName());
		bReturnValue = false;
	}

	if (!crossTraceComp) {
		UE_LOG(LogTemp, Warning, TEXT("%s: Pointer to crossTraceComp missed"), *GetName());
		bReturnValue = false;
	}

	return bReturnValue;
}

void AWeapon::SetFocus(const FVector & targetLoc) {
	FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetLoc);
	SetActorRotation(lookAtRot);
}

