// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Clip.h"
#include "PlayerDependency.h"
#include "../DragDrop/TimebarDragDropDependency.h"

UClip::UClip()
{
}

UClip::~UClip()
{
}

bool UClip::ShouldPlay(float CurrentTime)
{
	if (bIsPlayable && CurrentTime >= StartTime && CurrentTime < EndTime)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UClip::SetPlayable(bool InbIsPlayable)
{
	bIsPlayable = InbIsPlayable;
	if (TargetWidget)
	{
		TargetWidget->ExecSetPlayable(InbIsPlayable);
	}
	return InbIsPlayable;
}

void UClip::Play(float InCurrentTime)
{
	if (!bIsPlaying)
	{
		bIsPlaying = true;
	}

	if (bIsPlaying)
	{
		if (TargetWidget)
		{
			TargetWidget->ExecPlayClip();
		}
	}
	else
	{
		return;
	}
	//// 재생 시작 로그 출력
	//if (GEngine)
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, TEXT("Clip is now playing"));
	//}
}

void UClip::Pause()
{
	if (bIsPlaying)
	{
		bIsPlaying = false;
		// 일시 정지 로그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("Clip is paused"));
		}
	}
	else
	{
		return;
	}
}

void UClip::Stop()
{
	if (bIsPlaying)
	{
		bIsPlaying = false;

		if (TargetWidget)
		{
			TargetWidget->ExecStopClip();
		}

		// 정지 로그 출력
		/*if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Clip is stopped"));
		}*/
	}
	else
	{
		return;
	}
}
