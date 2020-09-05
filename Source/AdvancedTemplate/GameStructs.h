// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameStructs.generated.h"



USTRUCT(BlueprintType)
struct ADVANCEDTEMPLATE_API FAttribute {
	GENERATED_BODY()

protected:

	class UAttributesComponent * attrComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		FName name = "ATTRIBUTE_NAME";
	 
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float min = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float max = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float value = 100.0f;


	FAttribute() {}
	FAttribute(FName _name) : name(_name) {}
	FAttribute(FName _name, float _min, float _max, float _value) : name(_name), min(_min), max(_max), value(_value) {}

	void Init(class UAttributesComponent * _attrComp);

	void ClampValue() {
		if (min > max) {
			min = max;
			value = min;
		}
		else {
			value = FMath::Clamp(value, min, max);
		}
	}

	void AddValue(float addValue) {
		value += addValue;
		ClampValue();
		OnChange();
	}

	void Update(float addMin, float addMax, float addValue) {
		min += addMin;
		max += addMax;
		value += addValue;
		ClampValue();
		OnChange();
	}

	void OnChange();
	
	void SetName(FName newName) { name = newName; }
	void SetMin(float newMin) { min = newMin; }
	void SetMax(float newMax) { max = newMax; }
	void SetValue(float newValue) { value = newValue; }

	FORCEINLINE FName GetName() const { return name; }
	FORCEINLINE float GetMin() const { return min; }
	FORCEINLINE float GetMax() const { return max; }
	FORCEINLINE float GetValue() const { return value; }
};
/**
 * 
 */
UCLASS()
class ADVANCEDTEMPLATE_API UGameStructs : public UObject
{
	GENERATED_BODY()
	
};
