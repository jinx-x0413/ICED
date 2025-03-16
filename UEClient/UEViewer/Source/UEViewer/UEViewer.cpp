// Fill out your copyright notice in the Description page of Project Settings.

#include "UEViewer.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FUEViewer, UEViewer, "UEViewer" );


void FUEViewer::StartupModule()
{
	// 루트 모듈 초기화
	InitializeRootModule();
}

void FUEViewer::ShutdownModule()
{
	// 필요한 청소 작업 수행
	ShutdownAndUnloadModule(TEXT("AssetActor"));
	ShutdownAndUnloadModule(TEXT("AssetContent"));
	ShutdownAndUnloadModule(TEXT("Utility"));
	ShutdownAndUnloadModule(TEXT("Network"));
	ShutdownAndUnloadModule(TEXT("Command"));
	ShutdownAndUnloadModule(TEXT("UIComponent"));

	
}

void FUEViewer::InitializeRootModule()
{
	// 여러 서브 모듈을 로드하고 초기화
	RegisterSubModule(TEXT("AssetActor"));
	RegisterSubModule(TEXT("AssetContent"));
	RegisterSubModule(TEXT("Utility"));
	RegisterSubModule(TEXT("Network"));
	RegisterSubModule(TEXT("Command"));
	RegisterSubModule(TEXT("UIComponent"));
	
}

void FUEViewer::RegisterSubModule(FString ModuleName)
{
	FName ModuleFName(*ModuleName);  // FString -> FName 변환

	// ModuleClass를 기반으로 모듈을 로드합니다.
	if (!FModuleManager::Get().IsModuleLoaded(ModuleFName))
	{
		IModuleInterface* Module = FModuleManager::Get().LoadModule(ModuleFName);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load module %s"), *ModuleFName.ToString());
	}
}

void FUEViewer::ShutdownAndUnloadModule(FString ModuleName)
{
	FName ModuleFName(*ModuleName);  // FString -> FName 변환

	// 해당 모듈이 로드되어 있는지 확인
	if (FModuleManager::Get().IsModuleLoaded(ModuleFName))
	{
		// 로드된 모듈을 찾습니다.
		/*IModuleInterface* Module = FModuleManager::Get().GetModulePtr<IModuleInterface>(ModuleFName);

		if (Module)
		{
			Module->ShutdownModule();
		}

		FModuleManager::Get().UnloadModule(ModuleFName, true);*/
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Module %s is not loaded, cannot shutdown and unload."), *ModuleFName.ToString());
	}
}
