// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AdvancedTemplate/GameStructs.h"
#include "AttributesComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, const FAttribute&, changedAttribute);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEDTEMPLATE_API UAttributesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	FAttribute health = {"Health", 0, 100, 90};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
	FAttribute stamina = {"Stamina", 0, 100, 90 };

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeSignature OnAttributeChange;

	TArray<FAttribute*> attributes;

	// Sets default values for this component's properties
	UAttributesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void RegisterAttribute(FAttribute* attribute);

	FAttribute* FindAttribute(FName name);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void UpdateAttribute(FName name, float addMin, float addMax, float addValue);

	UFUNCTION(BlueprintCallable, Category = "Attribute")
	void AddValue(FName attributeName, float value);
};
