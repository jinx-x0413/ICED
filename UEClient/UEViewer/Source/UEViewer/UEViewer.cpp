// Fill out your copyright notice in the Description page of Project Settings.

#include "UEViewer.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FUEViewer, UEViewer, "UEViewer" );


void FUEViewer::StartupModule()
{
	// ��Ʈ ��� �ʱ�ȭ
	InitializeRootModule();
}

void FUEViewer::ShutdownModule()
{
	// �ʿ��� û�� �۾� ����
	ShutdownAndUnloadModule(TEXT("AssetActor"));
	ShutdownAndUnloadModule(TEXT("AssetContent"));
	ShutdownAndUnloadModule(TEXT("Utility"));
	ShutdownAndUnloadModule(TEXT("Network"));
	ShutdownAndUnloadModule(TEXT("Command"));
	ShutdownAndUnloadModule(TEXT("UIComponent"));

	
}

void FUEViewer::InitializeRootModule()
{
	// ���� ���� ����� �ε��ϰ� �ʱ�ȭ
	RegisterSubModule(TEXT("AssetActor"));
	RegisterSubModule(TEXT("AssetContent"));
	RegisterSubModule(TEXT("Utility"));
	RegisterSubModule(TEXT("Network"));
	RegisterSubModule(TEXT("Command"));
	RegisterSubModule(TEXT("UIComponent"));
	
}

void FUEViewer::RegisterSubModule(FString ModuleName)
{
	FName ModuleFName(*ModuleName);  // FString -> FName ��ȯ

	// ModuleClass�� ������� ����� �ε��մϴ�.
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
	FName ModuleFName(*ModuleName);  // FString -> FName ��ȯ

	// �ش� ����� �ε�Ǿ� �ִ��� Ȯ��
	if (FModuleManager::Get().IsModuleLoaded(ModuleFName))
	{
		// �ε�� ����� ã���ϴ�.
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
