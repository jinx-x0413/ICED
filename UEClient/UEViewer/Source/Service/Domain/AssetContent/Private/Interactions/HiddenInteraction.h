// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/InteractionBase.h"
#include "HiddenInteraction.generated.h"

/**
 * 
 */
UCLASS()
class ASSETCONTENT_API UHiddenInteraction : public UInteractionBase
{
	GENERATED_BODY()
	

	// Interaction overriding
public:
	virtual void Start() override;
	virtual void Finish() override;
	virtual void Reset() override;


};
