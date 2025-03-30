// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Track.h"
#include "PlayerDependency.h"
#include "../DragDrop/TimebarDragDropDependency.h"

UTrack::UTrack()
{
	if (!UTimebarPlayer::GetTimebarPlayer()->OnClipCreated.IsAlreadyBound(this, &UTrack::AddClipToArray))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnClipCreated.AddDynamic(this, &UTrack::AddClipToArray);
	}

	if (!UTimebarPlayer::GetTimebarPlayer()->OnClipDeleted.IsAlreadyBound(this, &UTrack::RemoveClip))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnClipDeleted.AddDynamic(this, &UTrack::RemoveClip);
	}
}

UTrack::~UTrack()
{
	if (UTimebarPlayer::GetTimebarPlayer()->OnClipCreated.IsAlreadyBound(this, &UTrack::AddClipToArray))
	{
		UTimebarPlayer::GetTimebarPlayer()->OnClipCreated.RemoveDynamic(this, &UTrack::AddClipToArray);
	}
}

void UTrack::AddClipToArray(UTrack* InTrack, UClip* InClip)
{
	if (IsValid(InClip) && InTrack == this)
	{
		Clips.Add(InClip);
	}
}

void UTrack::RemoveClip(UClip* InClip)
{
	for (auto& Clip : Clips)
	{
		if (Clip == InClip)
		{
			Clips.Remove(InClip);
			return;
		}
	}
}

void UTrack::SetClipsPlayable(bool InbIsPlayable)
{
	bIsPlayable = InbIsPlayable;
	if (ContentWidget)
	{
		ContentWidget->SetTrackPlayable(InbIsPlayable);
	}

	if (!Clips.IsEmpty())
	{
		for (auto& Clip : Clips)
		{
			Clip->SetPlayable(InbIsPlayable);
		}
	}

}

void UTrack::ToggleClipsPlayable()
{
	SetClipsPlayable(!bIsPlayable);
}

FVector2D UTrack::GetMinMaxTime()
{
	float min;
	float max;

	if (!Clips.IsEmpty())
	{
		min = Clips[0]->StartTime;
		max = Clips[0]->EndTime;

		for (auto& Clip : Clips)
		{
			if (Clip->StartTime < min)
			{
				min = Clip->StartTime;
			}

			if (Clip->EndTime > max)
			{
				max = Clip->EndTime;
			}
		}

		return FVector2D(min, max);
	}

	return FVector2D();
}
