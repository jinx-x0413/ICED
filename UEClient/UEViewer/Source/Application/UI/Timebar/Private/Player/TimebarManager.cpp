// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimebarManager.h"
#include "PlayerDependency.h"

UTimebarManager::UTimebarManager()
{
}

UTimebarManager::~UTimebarManager()
{
}

void UTimebarManager::BeginDestroy()
{
	Shutdown();

	Super::BeginDestroy();
}

void UTimebarManager::Shutdown()
{
	if (!TrackArray.IsEmpty())
	{
		for (auto& Track : TrackArray)
		{
			if (IsValid(Track) && Track->IsRooted())
			{
				Track->RemoveFromRoot();
				Track->MarkAsGarbage();
				Track = nullptr;
			}
		}
		TrackArray.Empty();
	}

	if (!ClipArray.IsEmpty())
	{
		for (auto& Clip : ClipArray)
		{
			if (IsValid(Clip) && Clip->IsRooted())
			{
				Clip->RemoveFromRoot();
				Clip->MarkAsGarbage();
				Clip = nullptr;
			}
		}
		ClipArray.Empty();
	}
}

void UTimebarManager::SetTracksVisibility(bool bIsVisible)
{
	if (!TrackArray.IsEmpty())
	{
		for (auto& Track : TrackArray)
		{
			if (IsValid(Track))
			{
				if (bIsVisible)
				{
					if (Track->HeaderWidget)
					{
						Track->HeaderWidget->SetVisibility(ESlateVisibility::Visible);
					}
					if (Track->ContentWidget)
					{
						Track->ContentWidget->SetVisibility(ESlateVisibility::Visible);
					}
					if (Track->SceneCaptureWidget)
					{
						Track->SceneCaptureWidget->SetVisibility(ESlateVisibility::Visible);
					}
					
				}
				else
				{
					if (Track->HeaderWidget)
					{
						Track->HeaderWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
					if (Track->ContentWidget)
					{
						Track->ContentWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
					if (Track->SceneCaptureWidget)
					{
						Track->SceneCaptureWidget->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
				
			}
		}
	}
}
