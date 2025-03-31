// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactions/InteractionBase.h"
#include "TransformInteraction.generated.h"

/**
 * 
 */
UCLASS()
class UTransformInteraction : public UInteractionBase
{
	GENERATED_BODY()
	

	// Interaction overriding
public:
	virtual void Initialize(FInteractionData InData) override;
	virtual void Play(float InCurrentTime) override; // for CurrentLocation
	virtual void Start() override;
	virtual void Finish() override;
	virtual void Reset() override;



	// feature
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CurrentLocation;

	//UFUNCTION(meta=(AllowPrivateAccess=true))
	FVector GetTargetActorCenter();
};
