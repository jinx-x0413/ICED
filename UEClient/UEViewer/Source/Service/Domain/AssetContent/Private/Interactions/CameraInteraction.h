// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/InteractionBase.h"
#include "CameraInteraction.generated.h"

/**
 * 
 */
UCLASS()
class UCameraInteraction : public UInteractionBase
{
	GENERATED_BODY()

	// Interaction overriding
public:
	virtual void Initialize(FInteractionData InData) override;
	virtual void Start() override;
	virtual void Finish() override;
	virtual void Reset() override;



	// feature
public:

	UFUNCTION(BlueprintCallable)
	void SetPawnTransform(FTransform TargetTransform, float InArmLength);
};
