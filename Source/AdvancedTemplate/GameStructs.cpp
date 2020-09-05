// Fill out your copyright notice in the Description page of Project Settings.


#include "GameStructs.h"
#include "AdvancedTemplate/Components/AttributesComponent.h"

void FAttribute::Init(UAttributesComponent * _attrComp){
	attrComp = _attrComp;
}

void FAttribute::OnChange(){
	if (attrComp) {
		attrComp->OnAttributeChange.Broadcast(*this);
	}
}
