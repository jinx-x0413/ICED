// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop/DroppableWidget.h"
#include "../../Private/DragDrop/DragDropDependency.h"


void UDroppableWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UDroppableWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

bool UDroppableWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UMyDragDropOperation* DragOp = Cast<UMyDragDropOperation>(InOperation))
	{
		if (UDraggableWidget* DraggedWidget = Cast<UDraggableWidget>(DragOp->SourceWidget))
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

bool UDroppableWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UMyDragDropOperation* DragOp = Cast<UMyDragDropOperation>(InOperation))
	{
		// 드래그가 활성화된 상태인지 확인
		UDraggableWidget* DraggedWidget = Cast<UDraggableWidget>(DragOp->SourceWidget);
		if (DraggedWidget)
		{
			FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
			FVector2D SnappedPosition = GetSnappedPosition(DropPosition);

			UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(DragOp->SourceWidget->Slot);
			if (PanelSlot)
			{
				FVector2D NewPosition = SnappedPosition - DragOp->DragOffset;
				PanelSlot->SetPosition(NewPosition);
			}

		}
	}

	return false;
}

FReply UDroppableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return FReply::Unhandled();
}

FVector2D UDroppableWidget::GetSnappedPosition(const FVector2D& InPosition)
{
	float SnappedX = FMath::RoundToFloat(InPosition.X / DragDropVars::SnapInterval) * DragDropVars::SnapInterval;
	float SnappedY = FMath::RoundToFloat(InPosition.Y / DragDropVars::SnapInterval) * DragDropVars::SnapInterval;

	return FVector2D(SnappedX, SnappedY);
}
