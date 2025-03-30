// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroppableWidget.generated.h"

/**
 * 
 */
UCLASS()
class UICOMPONENT_API UDroppableWidget : public UUserWidget
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

	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Snap")
	//float SnapInterval = 10.f;  // √÷º“ 10«»ºø ¥‹¿ß∑Œ Ω∫≥¿
	FVector2D  GetSnappedPosition(const FVector2D& InPosition);
};
