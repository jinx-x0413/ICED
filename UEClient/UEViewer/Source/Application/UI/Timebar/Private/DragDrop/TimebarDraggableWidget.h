// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimebarDraggableWidget.generated.h"

/**
 * 
 */
UCLASS()
class UTimebarDraggableWidget : public UUserWidget
{
	GENERATED_BODY()
	

	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;




	// DragDrop
public:
	FVector2D DraggedMousePosition;

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;


	//// snap drag
	bool bIsDragging = false;
	virtual FReply NativeOnMouseButtonUp(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;



};
