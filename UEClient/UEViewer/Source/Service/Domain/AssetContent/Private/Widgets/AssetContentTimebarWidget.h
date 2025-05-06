// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Application/UI/Timebar/Private/Player/TimebarWidget.h"
#include "AssetContentTimebarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UAssetContentTimebarWidget : public UTimebarWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;



	// callback
public:
	UFUNCTION()
	void OnComponentListItemCreated(class UTrack* InTrack, UUserWidget* InSceneCaptureWidget);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Component List Item Created"))
	void ExecOnComponentListItemCreated(UTrack* InTrack, UUserWidget* InSceneCaptureWidget);
};
