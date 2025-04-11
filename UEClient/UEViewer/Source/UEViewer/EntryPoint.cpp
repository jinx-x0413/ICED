// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryPoint.h"

void UEntryPoint::LoadActor()
{

}

UInteractionBase* UEntryPoint::CreateInteractionToTimebar(FInteractionData InInteractionData, UTrack* InTrack, float InStartTime, float InEndTime)
{
	// AssetContent :: Interaction
	UInteractionBase* ReturnInteraction = nullptr;
	if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetContent")))
	{
		FAssetContent* Module = FModuleManager::Get().GetModulePtr<FAssetContent>("AssetContent");
		if (Module)
		{
			ReturnInteraction = Module->Controller->CreateInteraction(InInteractionData, InTrack, InStartTime, InEndTime);
		}
	}


	// Timebar
	UTimebarPlayer::GetTimebarPlayer()->OnClipCreated.Broadcast(InTrack, ReturnInteraction);
	//UTimebarPlayer::GetTimebarPlayer()->OnTrackSelected.Broadcast(InTrack);

	return ReturnInteraction;
}

void UEntryPoint::BuildAssemblyContent(AActor* InActor, TSubclassOf<UUserWidget> InTrackHeaderWidgetClass, TSubclassOf<UUserWidget> InTrackWidgetClass)
{
	// Get Table Data
	FTableData CurrentTableData = UTableManager::GetTableManager()->TableData;

	// Build Content
	UInteractionBase* ReturnInteraction = nullptr;
	if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetContent")))
	{
		FAssetContent* Module = FModuleManager::Get().GetModulePtr<FAssetContent>("AssetContent");
		if (Module)
		{
			Module->Controller->TrackHeaderWidgetClass = InTrackHeaderWidgetClass;
			Module->Controller->TrackWidgetClass = InTrackWidgetClass;
			Module->Controller->BuildTemplate(ETemplateType::ASSEMBLY, InActor, CurrentTableData);
		}
	}
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