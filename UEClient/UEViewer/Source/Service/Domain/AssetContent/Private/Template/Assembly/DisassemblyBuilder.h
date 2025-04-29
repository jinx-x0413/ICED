// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../TemplateInterface.h"
#include "Blueprint/UserWidget.h"


#include "DisassemblyBuilder.generated.h"



class AGltfAssetActor;
class UTrack;
struct FActorHierarchyData;

UCLASS()
class UDisassemblyBuilder : public UObject, public ITemplateInterface
{
	GENERATED_BODY()

public:
	UDisassemblyBuilder();
	virtual ~UDisassemblyBuilder();
	virtual void BeginDestroy() override;



	// interface
public:
	void Build() override;



	// feature
public:
	float ClipInterval = 2.0f;

	float SetClip(
		UTrack* InTrack
		, TSubclassOf<UInteractionBase> InInteractionClass
		, FName InInteractionName
		, float PrevTrackEndTime
		, FString InFieldValue = "Auto"
		, TSubclassOf<class UUserWidget> InPopupWidgetClass = UUserWidget::StaticClass());

};
