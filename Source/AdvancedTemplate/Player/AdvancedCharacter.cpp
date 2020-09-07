// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvancedCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedTemplate\Components\CrosshairTraceComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Actors/Pickup.h"
//#include "Components/HealthComponent.h"
//#include "Player/AdvancedPlayerController.h"
//#include "Components/ItemManagerComponent.h"
#include "GameFramework/DamageType.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AAdvancedCharacter  

AAdvancedCharacter::AAdvancedCharacter() {

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(50.0f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	fpCameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	fpCameraComp->SetupAttachment(GetCapsuleComponent());
	fpCameraComp->SetRelativeLocation(FVector(0.0f, 0.0f, 64.f)); // Position the camera 
	fpCameraComp->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	meshFP = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	meshFP->SetOnlyOwnerSee(true);
	meshFP->SetupAttachment(fpCameraComp);
	meshFP->bCastDynamicShadow = false;
	meshFP->CastShadow = false;
	meshFP->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	meshFP->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	//Movement
	movementComp = GetCharacterMovement();
	movementComp->JumpZVelocity = 860.0f;
	movementComp->GravityScale = 3.0f;
	movementComp->MaxFlySpeed = 2000.0f;
	movementComp->BrakingDecelerationFlying = 2000.0f;
	movementComp->NavAgentProps.bCanCrouch = true;

	//Crosshair Trace
	crosshairTraceComp = CreateDefaultSubobject<UCrosshairTraceComponent>("Crosshair Trace Component");
	crosshairResult = crosshairTraceComp->GetCrosshairResultPtr();

	//Attributes
	//healthComp = CreateDefaultSubobject<UHealthComponent>("Health");

	//ItemManager
	//itemManagerComp = CreateDefaultSubobject<UItemManagerComponent>("ItemManager");
}

// Called when the game starts or when spawned
void AAdvancedCharacter::BeginPlay() {
	Super::BeginPlay();

	//UE_LOG(LogTemp, Warning, TEXT("Character %s Start Player Health: %s"), *attributeComp->GetName(), *attributeComp->health.ToString());

	capsuleBaseHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	cameraBaseLocation = fpCameraComp->GetRelativeLocation();
	standingCapsuleShape = GetCapsuleComponent()->GetCollisionShape();
	cRelativeCameraZOffset = 0.0f;

	//controllerAdv = APawn::GetController<AAdvancedPlayerController>();
	//controllerAdv->CreateWidgets();

	OnCharacterMovementUpdated.AddDynamic(this, &AAdvancedCharacter::OnMovementUpdate);
	OnTakeAnyDamage.AddDynamic(this, &AAdvancedCharacter::ReceiveDamageAny);

	meshFP->SetHiddenInGame(false, true);
}

/*
void AAdvancedCharacter::OnPickupTake(FItemData * itemData) {

	//Update Attributes from itemdata
	if (itemData) {
		itemManagerComp->AddItem(itemData);
	}
}
*/


// Called every frame
void AAdvancedCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAdvancedCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {

	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind sprint events
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AAdvancedCharacter::ActivateSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AAdvancedCharacter::DeactivateSprint);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AAdvancedCharacter::OnPressJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind jump events
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AAdvancedCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AAdvancedCharacter::StopCrouch);

	//Interaction
	//PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &AAdvancedCharacter::OnInteraction);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AAdvancedCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAdvancedCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAdvancedCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAdvancedCharacter::LookUpAtRate);
}



void AAdvancedCharacter::ReceiveDamageAny(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {

	//UE_LOG(LogTemp, Warning, TEXT("Player got %f damage"), Damage);
	//healthComp->ApplyDamage(Damage);
	//attributeComp->health->value -= Damage;

}


/*
void AAdvancedCharacter::OnInteraction() {
	if (crosshairResult->IsActorHit()) {
		APickup* pickup = crosshairResult->GetHitActor<APickup>();

		if (pickup) {
			OnPickupTake(pickup->GetItemData());
			pickup->Take(this);

		}
	}
}
*/

void AAdvancedCharacter::ActivateSprint() {
	if (bCanSprint) {
		bIsSprinting = true;
	}
}

void AAdvancedCharacter::DeactivateSprint() {
	bIsSprinting = false;
}

void AAdvancedCharacter::MoveForward(float Value) {

	if (Value != 0.0f) {
		// add movement in that direction 
		AddMovementInput(GetActorForwardVector(), Value);

		if (bCanSprint && bIsSprinting && Value > 0) {
			movementComp->MaxWalkSpeed = sprintSpeed;
			OnSprint();
			//healthComp->ConsumeStamina(sprintStaminaConsume);
			//attributeComp->stamina->value -= sprintStaminaConsume;
		}
		else {
			movementComp->MaxWalkSpeed = walkSpeed;
		}

	}

}

void AAdvancedCharacter::MoveRight(float Value) {
	if (Value != 0.0f) {
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);

		if (bIsSprinting) {
			//healthComp->ConsumeStamina(sprintStaminaConsume);
			//attributeComp->stamina->value -= sprintStaminaConsume;
		}

	}
}


void AAdvancedCharacter::TurnAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAdvancedCharacter::LookUpAtRate(float Rate) {
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

FVector AAdvancedCharacter::GetGroundLocation() const {

	float capsuleRadius, capsuleHalfHeight;
	GetCapsuleComponent()->GetScaledCapsuleSize(capsuleRadius, capsuleHalfHeight);

	FVector groundLoc = GetActorLocation();
	groundLoc.Z -= capsuleHalfHeight;

	return groundLoc;
}

float AAdvancedCharacter::TraceGroundDistance() const {

	FVector start = GetGroundLocation();

	FVector end = start;
	end.Z -= 10000.0f;

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);

	FHitResult traceResult;
	GetWorld()->LineTraceSingleByProfile(traceResult, start, end, GetCapsuleComponent()->GetCollisionProfileName(), traceParams);

	float distance = start.Z - traceResult.Location.Z;

	return distance;
}

bool AAdvancedCharacter::OverlapStandingCheck() const {
	FVector standingCapsuleLocation = GetCapsuleComponent()->GetComponentLocation();
	standingCapsuleLocation.Z += 96.0f - movementComp->CrouchedHalfHeight;

	FCollisionQueryParams CapsuleParams(SCENE_QUERY_STAT(CrouchTrace), false, this);
	FCollisionResponseParams ResponseParam;
	movementComp->InitCollisionParams(CapsuleParams, ResponseParam);

	const ECollisionChannel CollisionChannel = GetCapsuleComponent()->GetCollisionObjectType();

	bool bIsOverlapping = GetWorld()->OverlapBlockingTestByChannel(standingCapsuleLocation, FQuat::Identity, CollisionChannel, standingCapsuleShape, CapsuleParams, ResponseParam);

	DrawDebugCapsule(GetWorld(), standingCapsuleLocation, 96, GetCapsuleComponent()->GetScaledCapsuleRadius(), FQuat::Identity, FColor::Red, true, 60, 0, 1);

	return bIsOverlapping;
}

void AAdvancedCharacter::OnPressJump() {
	if (!movementComp->IsFalling()) {
		OnJump();
		OnJumpEvent.Broadcast();
	}

	ACharacter::Jump();
}

void AAdvancedCharacter::OnReleaseJump() {

}

void AAdvancedCharacter::AdjustCameraToCapsuleHeight(float deltaSeconds) {

	float cCapsuleHalfHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();

	float crouchShrinkPercent = cCapsuleHalfHeight / capsuleBaseHeight;

	float deltaHeight = capsuleBaseHeight - cCapsuleHalfHeight;

	float targetCamHeight = cameraBaseLocation.Z * crouchShrinkPercent;
	float cCamHeight = fpCameraComp->GetRelativeLocation().Z;
	FVector newCamLoc = fpCameraComp->GetRelativeLocation();


	float updateCamHeight = (targetCamHeight - cCamHeight) * crouchWalkSwitchSpeed * deltaSeconds;
	cRelativeCameraZOffset += updateCamHeight;

	newCamLoc.Z = cameraBaseLocation.Z + deltaHeight + cRelativeCameraZOffset;//Update Camera relative location to stay in place
	
	fpCameraComp->SetRelativeLocation(newCamLoc);

}

void AAdvancedCharacter::OnMovementUpdate(float DeltaSeconds, FVector OldLocation, FVector OldVelocity) {
	
		AdjustCameraToCapsuleHeight(DeltaSeconds);
	
}

void AAdvancedCharacter::StartCrouch() {

	if (!bIsCrouched) {
		Crouch();

		//Update cRelativeCameraZOffset to allow crouch while falling
		if (movementComp->IsFalling()) {
			cRelativeCameraZOffset -= GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2;
		}
	}
}

void AAdvancedCharacter::StopCrouch() {

	if (bIsCrouched) {
		UnCrouch();
	}

}

