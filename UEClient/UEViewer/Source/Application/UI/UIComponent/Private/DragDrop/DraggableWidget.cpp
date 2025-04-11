// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop/DraggableWidget.h"
#include "../../Private/DragDrop/DragDropDependency.h"

void UDraggableWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDraggableWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

FReply UDraggableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// check resizing
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(MousePosition); // Local space로 변환

		// reply for drag
		FEventReply Reply =
			UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

		return Reply.NativeReply;
	}
	return FReply::Unhandled();
}

void UDraggableWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	FVector2D DragStartPosition = InMouseEvent.GetScreenSpacePosition() - InGeometry.GetAbsolutePosition();

	UMyDragDropOperation* DragDropOp = NewObject<UMyDragDropOperation>();
	//DragDropOp->DefaultDragVisual = this;  // 드래그 중 보일 위젯 설정
	DragDropOp->SourceWidget = this;
	DragDropOp->DragOffset = DragStartPosition;
	DraggedMousePosition = DragStartPosition;

	OutOperation = DragDropOp; // return drag operation
}

FReply UDraggableWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	return FReply::Unhandled();
}
