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
void UEntryPoint::CreateGltfAssetActor(TSubclassOf<AGltfAssetActor> AssetActorClass, FTransform InTransform, UglTFRuntimeAsset* InAsset, const FString& FilePath, FString InFileName)
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("AssetActor")))
	{
		FAssetActor* Module = FModuleManager::Get().GetModulePtr<FAssetActor>("AssetActor");

		if (Module)
		{
			Module->Controller->CreateGltfAssetActor(AssetActorClass, InTransform, InAsset, FilePath, InFileName);
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

