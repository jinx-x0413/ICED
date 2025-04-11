// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryPoint.h"


// http
UHttpRequest* UEntryPoint::GetHttpRequest()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("HTTP_Connection")))
	{
		FHTTP_Connection* Module = FModuleManager::Get().GetModulePtr<FHTTP_Connection>("HTTP_Connection");
		if (Module)
		{
			if (Module->HttpRequest) // NULL 체크 추가
			{
				return Module->HttpRequest;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HttpRequest is NULL!"));
			}
		}
	}

	return nullptr;
}


void UEntryPoint::SendUserDataHttpRequest()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("HTTP_Connection")))
	{
		FHTTP_Connection* Module = FModuleManager::Get().GetModulePtr<FHTTP_Connection>("HTTP_Connection");
		if (Module)
		{
			if (Module->HttpRequest) // NULL 체크 추가
			{
				return Module->HttpRequest->SendUserDataHttpRequest();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HttpRequest is NULL!"));
			}
		}
	}
}

// asset actor
AGltfAssetActor* UEntryPoint::CreateGltfAssetActor(TSubclassOf<AGltfAssetActor> AssetActorClass, FTransform InTransform, UglTFRuntimeAsset* InAsset, const FString& FilePath, FString InFileName)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetActor")))
	{
		FAssetActor* Module = FModuleManager::Get().GetModulePtr<FAssetActor>("AssetActor");

		if (Module)
		{
			return Module->Controller->CreateGltfAssetActor(AssetActorClass, InTransform, InAsset, FilePath, InFileName);
		}
	}

	return nullptr;
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





// command
void UEntryPoint::Undo()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Command")))
	{
		FCommand* Module = FModuleManager::Get().GetModulePtr<FCommand>("Command");

		if (Module)
		{
			Module->Stack->Undo();
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


void UEntryPoint::StartSetAssetActorTransform(AActor* InActor, FTransform& InTransform)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Command")))
	{
		FCommand* Module = FModuleManager::Get().GetModulePtr<FCommand>("Command");

		if (Module)
		{
			FCommandData NewCommandData;
			NewCommandData.TargetActor = InActor;
			NewCommandData.OldTransform = InActor->GetActorTransform();
			UCommandBase* NewCommand = Module->Stack->CreateCommand(ECommandType::ASSETACTOR_TRANSFORM);
			NewCommand->Initialize(NewCommandData);
			// subscribe with observer
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


void UEntryPoint::EndSetAssetActorTransform(AActor* InActor, FTransform& InTransform)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Command")))
	{
		FCommand* Module = FModuleManager::Get().GetModulePtr<FCommand>("Command");

		if (Module)
		{
			UCommandBase* LastCommand = Module->Stack->GetLastCommand();
			FCommandData NewCommandData;
			NewCommandData.TargetActor = LastCommand->TargetData.TargetActor;
			NewCommandData.OldTransform = LastCommand->TargetData.OldTransform;
			NewCommandData.NewTransform = LastCommand->TargetData.TargetActor->GetActorTransform();
			LastCommand->Initialize(NewCommandData);
			// subscribe with observer
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
