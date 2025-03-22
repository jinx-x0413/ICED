// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragDrop/TimebarDraggableWidget.h"
#include "ClipWidget.generated.h"

/**
 * 
 */
UCLASS()
class UClipWidget : public UTimebarDraggableWidget
{
	GENERATED_BODY()


	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;



	// DragDrop
public:
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	//// snap drag
	virtual FReply NativeOnMouseButtonUp(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	//// resize
	bool bIsResizing = false;
	//float ResizeThreshold = 10.0f;

	virtual FReply NativeOnMouseMove(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ExecHoverStartResizing();

	UFUNCTION(BlueprintImplementableEvent)
	void ExecHoverStopResizing();




	// Player
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (ExposeOnSpawn = "true"))
	class UClip* TargetClip;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetConvertedValue();

	UFUNCTION()
	void SetClipTime(float InPositionX);

	UFUNCTION(BlueprintCallable)
	void SetClipLength(float InLeftPositionDiff, float InRightPositionDiff);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ExecSetClipLength(float InClipLength);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ExecPlayClip();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void ExecStopClip();


	//// Widget

	UFUNCTION()
	void SetActivated(UClip* InClip);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetActivated(bool bIsActivated);

	UFUNCTION()
	void RemoveClip(UClip* InClip);

	UFUNCTION()
	void SetClipScale(float InOldScale, float InNewScale);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetPlayable(bool InbIsPlayable);
};
