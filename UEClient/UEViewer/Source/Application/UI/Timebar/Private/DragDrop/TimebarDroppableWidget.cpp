// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop/TimebarDroppableWidget.h"
#include "TimebarDragDropDependency.h"
#include "../Player/PlayerDependency.h"

// construct
void UTimebarDroppableWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTimebarDroppableWidget::NativeDestruct()
{
	Super::NativeDestruct();
}





// DragDrop
bool UTimebarDroppableWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{

	if (UTimebarDragDropOperation* DragOp = Cast<UTimebarDragDropOperation>(InOperation))
	{
		if (UTimebarDraggableWidget* DraggedWidget = Cast<UTimebarDraggableWidget>(DragOp->SourceWidget))
		{
			FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
			FVector2D SnappedPosition = GetSnappedPosition(DropPosition);  // 예: 스냅 위치 계산

			UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(DraggedWidget->Slot);
			if (PanelSlot)
			{	
				FVector2D NewPosition = FVector2D(SnappedPosition.X - DragOp->DragOffset.X, PanelSlot->GetPosition().Y);
				PanelSlot->SetPosition(NewPosition);
			}
			return true;
		}
	}

	return false;
}

FVector2D UTimebarDroppableWidget::GetSnappedPosition(const FVector2D& InPosition)
{
	float SnappedX = FMath::RoundToFloat(InPosition.X / Variables::SnapInterval) * Variables::SnapInterval;
	//float SnappedY = FMath::RoundToFloat(InPosition.Y / Variables::SnapInterval) * Variables::SnapInterval;
	float SnappedY = FMath::RoundToFloat(InPosition.Y / 60.0f) * 60.0f;

	//return FVector2D(SnappedX, InPosition.Y);
	return FVector2D(SnappedX, SnappedY);
}

bool UTimebarDroppableWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UTimebarDragDropOperation* DragOp = Cast<UTimebarDragDropOperation>(InOperation))
	{
		// 드래그가 활성화된 상태인지 확인
		UClipWidget* DraggedWidget = Cast<UClipWidget>(DragOp->SourceWidget);
		if (DraggedWidget)
		{
			FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
			FVector2D SnappedPosition = GetSnappedPosition(DropPosition);

			UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(DragOp->SourceWidget->Slot);
			if (PanelSlot)
			{
				FVector2D NewPosition = FVector2D(SnappedPosition.X - DragOp->DragOffset.X, PanelSlot->GetPosition().Y);
				PanelSlot->SetPosition(NewPosition);
			}

		}
	}

	return false;
}


FReply UTimebarDroppableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}
