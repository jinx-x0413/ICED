// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragDrop/Track/TrackWidget.h"
#include "SideTrackWidget.generated.h"

/**
 * 
 */
UCLASS()
class USideTrackWidget : public UTrackWidget
{
	GENERATED_BODY()
	
	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;




	// Override Track Widget Function
public:
	virtual void SetTrackPlayable(bool InbIsPlayable) override;



	// refer main track
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTrack* ParentTrack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTrackWidget* ParentTrackWidget;



	// side track function
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ExecCreateSideTrack(UTrack* InParentTrack, UTrack* InTrack, UUserWidget* InHeaderWidget, USideTrackWidget* InContentTrack);
};
