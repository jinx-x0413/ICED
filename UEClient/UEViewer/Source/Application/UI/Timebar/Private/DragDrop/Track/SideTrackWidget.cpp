// Fill out your copyright notice in the Description page of Project Settings.


#include "DragDrop/Track/SideTrackWidget.h"
#include "../TimebarDragDropDependency.h"
#include "../../Player/PlayerDependency.h"

void USideTrackWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!UTimebarPlayer::GetTimebarPlayer()->OnSideTrackCreated.IsAlreadyBound(this, &USideTrackWidget::ExecCreateSideTrack))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnSideTrackCreated.AddDynamic(this, &USideTrackWidget::ExecCreateSideTrack);
	}
}

void USideTrackWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (UTimebarPlayer::GetTimebarPlayer()->OnSideTrackCreated.IsAlreadyBound(this, &USideTrackWidget::ExecCreateSideTrack))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnSideTrackCreated.RemoveDynamic(this, &USideTrackWidget::ExecCreateSideTrack);
	}
}

void USideTrackWidget::SetTrackPlayable(bool InbIsPlayable)
{
	ExecSetTrackPlayable(InbIsPlayable);
}
