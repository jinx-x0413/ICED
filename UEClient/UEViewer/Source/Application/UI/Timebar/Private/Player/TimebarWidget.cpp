// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimebarWidget.h"
#include "PlayerDependency.h"
#include "../DragDrop/TimebarDragDropDependency.h"

void UTimebarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.IsAlreadyBound(this, &UTimebarWidget::Run))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.AddDynamic(this, &UTimebarWidget::Run);
	}
}

void UTimebarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (!UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.IsAlreadyBound(this, &UTimebarWidget::Run))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.RemoveDynamic(this, &UTimebarWidget::Run);
	}
}

FReply UTimebarWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UTimebarPlayer::GetTimebarPlayer()->OnTrackSelected.Broadcast(nullptr);
	UTimebarPlayer::GetTimebarPlayer()->OnClipSelected.Broadcast(nullptr);
	return FReply::Unhandled();
}

void UTimebarWidget::Run(float InCurrentTime)
{
	ExecRun(InCurrentTime, Variables::ConvertedValue * Variables::ScaleMultiplier);

}

void UTimebarWidget::SetScrollOffset(UScrollBox* InScrollBox, float InCurrentTime)
{
	if (InCurrentTime < InScrollBox->GetScrollOffset())
	{
		// 커서가 왼쪽에 벗어나면, 스크롤을 왼쪽으로 이동
		InScrollBox->SetScrollOffset(InCurrentTime);
	}
	else if (InCurrentTime > (InScrollBox->GetScrollOffset() + InScrollBox->GetCachedGeometry().GetLocalSize().X) - Variables::CursorInterval)
	{
		// 커서가 오른쪽에 벗어나면, 스크롤을 오른쪽으로 이동
		InScrollBox->SetScrollOffset(InCurrentTime - InScrollBox->GetCachedGeometry().GetLocalSize().X + Variables::CursorInterval);
	}
	
}
