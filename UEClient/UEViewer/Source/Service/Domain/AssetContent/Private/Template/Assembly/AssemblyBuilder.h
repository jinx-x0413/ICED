// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../TemplateInterface.h"
#include "Blueprint/UserWidget.h"

#include "AssemblyBuilder.generated.h"


class AGltfAssetActor;
class UTrack;
struct FActorHierarchyData;


UCLASS()
class UAssemblyBuilder : public UObject, public ITemplateInterface
{
	GENERATED_BODY()

	// construct
public:
	UAssemblyBuilder();
	virtual ~UAssemblyBuilder();
	virtual void BeginDestroy() override;


	// interface
public:
	virtual void Build() override;





	// feature
public:
	float ClipInterval = 1.0f;
	//float TrackInterval = 8.0f;

	float SetClip(
		UTrack* InTrack
		, TSubclassOf<UInteractionBase> InInteractionClass
		, FName InInteractionName
		, float PrevTrackEndTime
		, FString InFieldValue = "Auto"
		, TSubclassOf<class UUserWidget> InPopupWidgetClass = UUserWidget::StaticClass());

};
