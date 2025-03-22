// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragDrop/TimebarDroppableWidget.h"
#include "TrackWidget.generated.h"

/**
 * 
 */
UCLASS()
class UTrackWidget : public UTimebarDroppableWidget
{
	GENERATED_BODY()


	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;



	// DragDrop
public:
	bool NativeOnDrop(
		const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation);

	//const float MinClipLength = 30.0f; // 최소 클립 길이 설정

	//// snap drag / resize
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;



	// Player
public:
	//// run
	UFUNCTION(BlueprintImplementableEvent)
	void ExecRun(float DeltaTime);


	//// track
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTrack* TargetTrack;

	UFUNCTION(BlueprintImplementableEvent)
	void ExecCreateTrack(UTrack* InTrack, UUserWidget* InHeaderWidget, UTrackWidget* InContentWidget);

	UFUNCTION()
	virtual void SetTrackPlayable(bool InbIsPlayable);
	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetTrackPlayable(bool InbIsPlayable);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHidden = false;

	UFUNCTION()
	void SetActivated(UTrack* InTrack);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetActivated(bool bIsActivated);


	//// clip
	UFUNCTION(BlueprintCallable)
	void InitializeClipWidget(UClipWidget* InClipWidget, UClip* InClip);


	UFUNCTION()
	FVector2D SetPossibleClipPosition(UClipWidget* InDraggableWidget, FVector2D InNewPosition);

	UFUNCTION()
	float SetPossibleDelta(UClipWidget* InDraggableWidget, float InDelta);

	
	
};
