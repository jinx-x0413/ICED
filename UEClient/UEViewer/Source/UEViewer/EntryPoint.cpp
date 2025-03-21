// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryPoint.h"

void UEntryPoint::LoadActor()
{

}

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
