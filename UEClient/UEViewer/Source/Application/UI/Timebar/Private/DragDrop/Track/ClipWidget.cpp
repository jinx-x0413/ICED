// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop/Track/ClipWidget.h"
#include "../TimebarDragDropDependency.h"
#include "../../Player/PlayerDependency.h"

// construct
void UClipWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.IsAlreadyBound(this, &UClipWidget::SetActivated))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.AddDynamic(this, &UClipWidget::SetActivated);
	}

	if (!UTimebarPlayer::GetTimebarPlayer()->OnClipDeleted.IsAlreadyBound(this, &UClipWidget::RemoveClip))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnClipDeleted.AddDynamic(this, &UClipWidget::RemoveClip);
	}

	if (!UTimebarPlayer::GetTimebarPlayer()->OnScaleChanged.IsAlreadyBound(this, &UClipWidget::SetClipScale))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnScaleChanged.AddDynamic(this, &UClipWidget::SetClipScale);
	}
}

void UClipWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.IsAlreadyBound(this, &UClipWidget::SetActivated))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.RemoveDynamic(this, &UClipWidget::SetActivated);
	}

	if (UTimebarPlayer::GetTimebarPlayer()->OnClipDeleted.IsAlreadyBound(this, &UClipWidget::RemoveClip))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnClipDeleted.RemoveDynamic(this, &UClipWidget::RemoveClip);
	}

	if (UTimebarPlayer::GetTimebarPlayer()->OnScaleChanged.IsAlreadyBound(this, &UClipWidget::SetClipScale))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnScaleChanged.RemoveDynamic(this, &UClipWidget::SetClipScale);
	}

	if (IsValid(TargetClip) && TargetClip->IsRooted())
	{
		TargetClip->RemoveFromRoot();
		TargetClip->MarkAsGarbage();
		TargetClip = nullptr;
	}
}





// DragDrop
FReply UClipWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// set activated
		UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.Broadcast(TargetClip);

		// check resizing
		FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
		FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(MousePosition); // Local space로 변환

		FVector2D ClipSize = GetDesiredSize();
		if (TargetClip)
		{
			ClipSize.X = TargetClip->ClipLength * Variables::ConvertedValue * Variables::ScaleMultiplier;
		}


		if (LocalMousePosition.X >= ClipSize.X - Variables::ResizeThreshold)
		{
			bIsDragging = false;
			bIsResizing = true;
		}
		else
		{
			bIsDragging = true;
			bIsResizing = false;
		}

		// reply for drag
		FEventReply Reply =
			UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);

		return Reply.NativeReply;
	}

	return FReply::Unhandled();
}

void UClipWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	FVector2D DragStartPosition = InMouseEvent.GetScreenSpacePosition() - InGeometry.GetAbsolutePosition();

	UTimebarDragDropOperation* DragDropOp = NewObject<UTimebarDragDropOperation>();
	//DragDropOp->DefaultDragVisual = this;  // 드래그 중 보일 위젯 설정

	if (bIsDragging)
	{
		DragDropOp->Pivot = EDragPivot::TopLeft;  // move 드래그 시작점 지정
	}
	else if (bIsResizing)
	{
		DragDropOp->Pivot = EDragPivot::BottomRight;  // resize 드래그 시작점 지정
	}

	DragDropOp->SourceWidget = this;
	DragDropOp->DragOffset = DragStartPosition;
	DraggedMousePosition = DragStartPosition;

	OutOperation = DragDropOp; // return drag operation


}

//// snap
FReply UClipWidget::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	bIsResizing = false;

	if (TargetClip)
	{
		UTimebarPlayer::GetTimebarPlayer()->SetCurrentTime(TargetClip->StartTime);
	}
	
	return FReply::Unhandled();
}


//// resize
FReply UClipWidget::NativeOnMouseMove(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDragging) {
		return FReply::Unhandled();
	}

	FVector2D MousePosition = InMouseEvent.GetScreenSpacePosition();
	FVector2D LocalMousePosition = InGeometry.AbsoluteToLocal(MousePosition); // Convert to local space

	FVector2D ClipSize = GetDesiredSize();
	if (TargetClip)
	{
		ClipSize.X = TargetClip->ClipLength * Variables::ConvertedValue * Variables::ScaleMultiplier;
	}

	// Check if the mouse is within the resize area
	if (LocalMousePosition.X >= ClipSize.X - Variables::ResizeThreshold)
	{
		// Call ExecHoverResizing when inside the threshold
		ExecHoverStartResizing();
	}
	else
	{
		ExecHoverStopResizing();
	}

	return FReply::Unhandled();
}

void UClipWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	ExecHoverStopResizing();
}




float UClipWidget::GetConvertedValue()
{
	return Variables::ConvertedValue * Variables::ScaleMultiplier;
}

// Player
void UClipWidget::SetClipTime(float InPositionX)
{
	if (TargetClip)
	{
		TargetClip->StartTime = InPositionX / (Variables::ConvertedValue * Variables::ScaleMultiplier);
		TargetClip->EndTime = (InPositionX / (Variables::ConvertedValue * Variables::ScaleMultiplier)) + TargetClip->ClipLength;
	}
}

void UClipWidget::SetClipLength(float InLeftPositionDiff, float InRightPositionDiff)
{
	if (TargetClip)
	{
		TargetClip->ClipLength += InRightPositionDiff / (Variables::ConvertedValue * Variables::ScaleMultiplier);
		TargetClip->EndTime = TargetClip->StartTime + TargetClip->ClipLength;

		ExecSetClipLength(TargetClip->ClipLength * Variables::ConvertedValue * Variables::ScaleMultiplier);

	}
}


void UClipWidget::SetActivated(UClip* InClip)
{
	if (TargetClip && InClip == TargetClip)
	{
		ExecSetActivated(true);
	}
	else
	{
		ExecSetActivated(false);
	}
}

void UClipWidget::RemoveClip(UClip* InClip)
{
	if (IsValid(TargetClip) && TargetClip == InClip)
	{
		TargetClip->RemoveFromRoot();
		TargetClip->MarkAsGarbage();
		TargetClip = nullptr;

		RemoveFromParent();
	}
}

void UClipWidget::SetClipScale(float InOldScale, float InNewScale)
{
	ExecSetClipLength(TargetClip->ClipLength * Variables::ConvertedValue * InNewScale);
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (PanelSlot)
	{
		if (PanelSlot->GetPosition().X != 0) // 0인 경우 제자리
		{
			PanelSlot->SetPosition(FVector2D(PanelSlot->GetPosition().X * (InNewScale/InOldScale), PanelSlot->GetPosition().Y));
		}


	}
}
