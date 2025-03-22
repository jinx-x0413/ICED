// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackWidget.h"
#include "../TimebarDragDropDependency.h"
#include "../../Player/PlayerDependency.h"

// construct
void UTrackWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UTimebarPlayer::GetTimebarPlayer()->OnTrackSelected.IsAlreadyBound(this, &UTrackWidget::SetActivated))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnTrackSelected.AddDynamic(this, &UTrackWidget::SetActivated);
	}

	if (!UTimebarPlayer::GetTimebarPlayer()->OnTrackCreated.IsAlreadyBound(this, &UTrackWidget::ExecCreateTrack))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnTrackCreated.AddDynamic(this, &UTrackWidget::ExecCreateTrack);
	}

	// OnCurrentTimeChanged AddDynamic : Run
	if (!UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.IsAlreadyBound(this, &UTrackWidget::ExecRun))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.AddDynamic(this, &UTrackWidget::ExecRun);
	}
}

void UTrackWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (IsValid(TargetTrack) && TargetTrack->IsRooted())
	{
		TargetTrack->RemoveFromRoot();
		TargetTrack->MarkAsGarbage();
		TargetTrack = nullptr;
	}

	if (UTimebarPlayer::GetTimebarPlayer()->OnTrackSelected.IsAlreadyBound(this, &UTrackWidget::SetActivated))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnTrackSelected.RemoveDynamic(this, &UTrackWidget::SetActivated);
	}

	if (UTimebarPlayer::GetTimebarPlayer()->OnTrackCreated.IsAlreadyBound(this, &UTrackWidget::ExecCreateTrack))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnTrackCreated.RemoveDynamic(this, &UTrackWidget::ExecCreateTrack);
	}

	// OnCurrentTimeChanged RemoveDynamic : Run
	if (UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.IsAlreadyBound(this, &UTrackWidget::ExecRun))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.RemoveDynamic(this, &UTrackWidget::ExecRun);
	}
}




// DragDrop
bool UTrackWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UTimebarDragDropOperation* DragOp = Cast<UTimebarDragDropOperation>(InOperation))
	{
		if (UClipWidget* DraggedWidget = Cast<UClipWidget>(DragOp->SourceWidget))
		{
			DraggedWidget->bIsDragging = false;
			DraggedWidget->bIsResizing = false;
			return true;
		}
	}
	return false;
}

bool UTrackWidget::NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (UTimebarDragDropOperation* DragOp = Cast<UTimebarDragDropOperation>(InOperation))
	{
		// 드래그가 활성화된 상태인지 확인
		UClipWidget* DraggedWidget = Cast<UClipWidget>(DragOp->SourceWidget);
		if (DraggedWidget && (DraggedWidget->bIsDragging || DraggedWidget->bIsResizing))
		{
			FVector2D DropPosition = InGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());
			FVector2D SnappedPosition = GetSnappedPosition(DropPosition);

			UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(DragOp->SourceWidget->Slot);
			if (PanelSlot)
			{

				if (DraggedWidget->bIsDragging)
				{
					// Snap 간격에 맞춰 위치 조정
					FVector2D NewPosition = FVector2D(SnappedPosition.X - DragOp->DragOffset.X, PanelSlot->GetPosition().Y);

					// 다른 클립과 겹치는지 확인
					NewPosition = SetPossibleClipPosition(DraggedWidget, NewPosition);

					PanelSlot->SetPosition(NewPosition);
					DraggedWidget->SetClipTime(NewPosition.X); // set clip time
				}
				else if (DraggedWidget->bIsResizing)
				{
					FVector2D PanelPosition = PanelSlot->GetPosition();
					float Delta = SnappedPosition.X - DraggedWidget->DraggedMousePosition.X;
					Delta = FMath::Clamp(Delta, -Variables::SnapInterval, Variables::SnapInterval);
					Delta = SetPossibleDelta(DraggedWidget, Delta);

					if (DraggedWidget->TargetClip && (DraggedWidget->TargetClip->ClipLength * Variables::ConvertedValue * Variables::ScaleMultiplier + Delta) >= Variables::MinClipLength)
					{
						DraggedWidget->SetClipLength(0.f, Delta);
					}

					DraggedWidget->DraggedMousePosition.X = SnappedPosition.X;

				}

			}

		}
	}

	return false;
}

FReply UTrackWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.Broadcast(nullptr);
	UTimebarPlayer::GetTimebarPlayer()->OnTrackSelected.Broadcast(TargetTrack);

	
	return FReply::Handled();
}




// player
void UTrackWidget::SetTrackPlayable(bool InbIsPlayable)
{
	// Set SideTracks Playable
	for (auto& SideTrack : TargetTrack->SideTracks)
	{
		//SideTrack->ContentWidget->SetTrackPlayable(InbIsPlayable);
		SideTrack->SetClipsPlayable(InbIsPlayable);
	}

	ExecSetTrackPlayable(InbIsPlayable);
}

void UTrackWidget::SetActivated(UTrack* InTrack)
{
	if (TargetTrack && TargetTrack == InTrack)
	{
		ExecSetActivated(true);
	}
	else
	{
		ExecSetActivated(false);
	}
}

void UTrackWidget::InitializeClipWidget(UClipWidget* InClipWidget, UClip* InClip)
{
	
	UCanvasPanelSlot* PanelSlot = Cast<UCanvasPanelSlot>(InClipWidget->Slot);
	if (!PanelSlot)
	{
		return;
	}

	// set target clip widget to clip
	InClip->TargetWidget = InClipWidget;
	InClipWidget->TargetClip = InClip;

	// set initial position
	//FVector2D CurrentConvertedPosition = FVector2D(UTimebarPlayer::GetTimebarPlayer()->CurrentTime * Variables::ConvertedValue * Variables::ScaleMultiplier, 0.0f);
	FVector2D CurrentConvertedPosition = FVector2D(InClip->StartTime * Variables::ConvertedValue * Variables::ScaleMultiplier, 0);
	FVector2D NewPosition = SetPossibleClipPosition(InClipWidget, CurrentConvertedPosition);
	
	if (NewPosition.X == 0.0f ) //// current time impossible
	{
		float MaxFrame = 0.0f;
		for (auto& Clip : TargetTrack->Clips)
		{
			if (Clip != InClip && Clip->EndTime >= MaxFrame)
			{
				MaxFrame = Clip->EndTime;
			}
		}
		InClip->StartTime = MaxFrame;
		InClip->EndTime = InClip->ClipLength + MaxFrame;

		NewPosition = FVector2D(InClip->StartTime * Variables::ConvertedValue * Variables::ScaleMultiplier, PanelSlot->GetPosition().Y);
	}
	else 	//// current time possible
	{
		InClip->StartTime = NewPosition.X / (Variables::ConvertedValue * Variables::ScaleMultiplier);
		InClip->EndTime = InClip->StartTime + InClip->ClipLength;
	}

	if (PanelSlot)
	{
		PanelSlot->SetPosition(NewPosition);
	}

	//UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.Broadcast(InClip);

}

FVector2D UTrackWidget::SetPossibleClipPosition(UClipWidget* InDraggableWidget, FVector2D InNewPosition)
{

	FVector2D ReturnVector = InNewPosition;
	bool bIsOverlapping = false;

	//if (InDraggableWidget->bIsDragging && !TargetTrack->Clips.IsEmpty())
	if (!TargetTrack->Clips.IsEmpty())
	{
		float TargetPositionX = InNewPosition.X / (Variables::ConvertedValue *Variables::ScaleMultiplier);

		// 트랙을 벗어남
		if (TargetPositionX < 0.0f)
		{
			bIsOverlapping = true;
		}
		else
		{
			for (auto& Clip : TargetTrack->Clips)
			{
				if (Clip != InDraggableWidget->TargetClip)
				{

					if (TargetPositionX >= Clip->StartTime && TargetPositionX <= Clip->EndTime) // 새로운 클립의 앞(Head)이 기존 위젯과 겹침
					{
						bIsOverlapping = true;
						break;
					}
					else if (TargetPositionX + InDraggableWidget->TargetClip->ClipLength >= Clip->StartTime
						&& TargetPositionX + InDraggableWidget->TargetClip->ClipLength <= Clip->EndTime) // 새로운 클립의 뒤(Tail)가 기존 위젯과 겹침
					{
						bIsOverlapping = true;
						break;
					}
					else if (TargetPositionX <= Clip->StartTime && TargetPositionX + InDraggableWidget->TargetClip->ClipLength >= Clip->EndTime) // 새로운 클립이 기존 위젯을 포함하면서 겹침
					{
						bIsOverlapping = true;
						break;
					}
				}
			}
		}

	}

	if (bIsOverlapping)
	{
		return FVector2D(InDraggableWidget->TargetClip->StartTime * Variables::ConvertedValue* Variables::ScaleMultiplier, InNewPosition.Y);
	}

	return ReturnVector;

}

float UTrackWidget::SetPossibleDelta(UClipWidget* InDraggableWidget, float InDelta)
{
	float ReturnValue = InDelta;

	if (InDraggableWidget->bIsResizing && !TargetTrack->Clips.IsEmpty())
	{
		float TargetStartX = InDraggableWidget->TargetClip->StartTime;
		float TargetEndX = TargetStartX + InDraggableWidget->TargetClip->ClipLength + (InDelta / (Variables::ConvertedValue * Variables::ScaleMultiplier));

		for (auto& Clip : TargetTrack->Clips)
		{
			if (Clip != InDraggableWidget->TargetClip)
			{
				if (TargetStartX < Clip->StartTime && TargetEndX >= Clip->StartTime) // 새로운 클립의 앞(Head)이 기존 위젯과 겹침
				{
					ReturnValue = 0;
					break;
				}

			}
		}
	}


	return ReturnValue;
}
