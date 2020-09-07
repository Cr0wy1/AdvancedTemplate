// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributesComponent.h"

// Sets default values for this component's properties
UAttributesComponent::UAttributesComponent(){
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAttributesComponent::BeginPlay(){
	Super::BeginPlay();

	RegisterAttribute(&health);
	RegisterAttribute(&stamina);
}

void UAttributesComponent::RegisterAttribute(FAttribute * attribute){
	if (attribute) {
		attributes.AddUnique(attribute);
		attribute->Init(this);
	}
}

FAttribute * UAttributesComponent::FindAttribute(FName name){
	for (auto attr : attributes) {
		if (attr->GetName() == name) {
			return attr;
		}
	}
	return nullptr;
}


// Called every frame
void UAttributesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}


void UAttributesComponent::UpdateAttribute(FName name, float addMin, float addMax, float addValue, FAttribute &updatedAttribute, bool &success){
	auto attr = FindAttribute(name);
	if (attr) {
		attr->Update(addMin, addMax, addValue);
		updatedAttribute = *attr;
		success = true;
		return;
	}
	success = false;
}

void UAttributesComponent::AddValue(FName attributeName, float value, float &updatedValue, float &success){
	auto attr = FindAttribute(attributeName);
	if (attr) {
			attr->AddValue(value);
			updatedValue = attr->value;
			success = true;
			return;
	}
	success = false;
}

