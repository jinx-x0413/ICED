// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../TemplateInterface.h"

#include "ComponentBuilder.generated.h"

/**
 * 
 */
UCLASS()
class UComponentBuilder : public UObject, public ITemplateInterface
{
	GENERATED_BODY()

	// construct
public:
	UComponentBuilder();
	virtual ~UComponentBuilder();



	// interface
public:
	virtual void Build() override;





	// feature
public:
	//void SetTracks(FActorHierarchyData InHierarchyData, float PrevTrackEndTime);
	float ClipInterval = 2.0f;
	float SetClip(
		UTrack* InTrack
		, TSubclassOf<UInteractionBase> InInteractionClass
		, FName InInteractionName
		, float PrevTrackEndTime
		, FString InFieldValue = "Auto");

	// TODO : refactoring
	float SetClipBackward(
		UTrack* InTrack
		, TSubclassOf<UInteractionBase> InInteractionClass
		, FName InInteractionName
		, float PrevTrackEndTime
		, FString InFieldValue = "Auto");
};
