// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryPoint.h"

void UEntryPoint::LoadActor()
{

}



// Timebar

void UEntryPoint::CreateTrackToTimebar(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->CreateTrack(InHeaderWidget, InContentWidget, InName);
		}
	}
}

void UEntryPoint::DeleteTrackFromTimebar(UUserWidget* InTrackWidget)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->DeleteTrack(InTrackWidget);
		}
	}
}

void UEntryPoint::CreateClipToTimebar(UUserWidget* InTrackWidget, float InStartTime, float InEndTime, FString InName)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->CreateClip(InTrackWidget, InStartTime, InEndTime, InName);
		}
	}
}

void UEntryPoint::DeleteClipFromTimebar(UUserWidget* InClipWidget)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->DeleteClip(InClipWidget);
		}
	}
}

void UEntryPoint::StartTimebar()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->Start();
		}
	}
}

void UEntryPoint::PauseTimebar()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->Pause();
		}
	}
}

void UEntryPoint::StopTimebar()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->Stop();
		}
	}
}

void UEntryPoint::SetTimebarCurrentTime(float InCurrentTime)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->SetCurrentTime(InCurrentTime);
		}
	}
}