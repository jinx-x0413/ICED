// Fill out your copyright notice in the Description page of Project Settings.


#include "TimebarController.h"
#include "TimebarDependency.h"

// Construct
UTimebarController::UTimebarController()
{
}

UTimebarController::~UTimebarController()
{
}




// Business Logic
void UTimebarController::CreateTrack(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName)
{
	bool bIsSideTrack = InContentWidget->IsChildOf(USideTrackWidget::StaticClass());
	if (bIsSideTrack && UTimebarPlayer::GetTimebarPlayer()->SelectedTrack)
	{
		// create side track
		UTimebarPlayer::GetTimebarPlayer()->CreateSideTrack(UTimebarPlayer::GetTimebarPlayer()->SelectedTrack, InHeaderWidget, InContentWidget, InName);
	}
	else
	{
		UTimebarPlayer::GetTimebarPlayer()->CreateTrack(InHeaderWidget, InContentWidget, InName);
	}
}

void UTimebarController::DeleteTrack(UUserWidget* InTrackWidget)
{
	UTimebarPlayer::GetTimebarPlayer()->DeleteTrack(Cast<UTrackWidget>(InTrackWidget)->TargetTrack);
}

void UTimebarController::CreateClip(UUserWidget* InTrackWidget, float InStartTime, float InEndTime, FString InName)
{
	UTimebarPlayer::GetTimebarPlayer()->CreateClip(Cast<UTrackWidget>(InTrackWidget)->TargetTrack, InStartTime, InEndTime, InName);
	
}

void UTimebarController::DeleteClip(UUserWidget* InClipWidget)
{
	
	UTimebarPlayer::GetTimebarPlayer()->DeleteClip(Cast<UClipWidget>(InClipWidget)->TargetClip);
}

void UTimebarController::Start()
{
	UTimebarPlayer::GetTimebarPlayer()->Start();
}

void UTimebarController::Pause()
{
	UTimebarPlayer::GetTimebarPlayer()->Pause();
}

void UTimebarController::Stop()
{
	UTimebarPlayer::GetTimebarPlayer()->Stop();
}

void UTimebarController::SetCurrentTime(float InCurrentTime)
{
	UTimebarPlayer::GetTimebarPlayer()->CurrentTime = InCurrentTime;
	UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.Broadcast(InCurrentTime);
}
