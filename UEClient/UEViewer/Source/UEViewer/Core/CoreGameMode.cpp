// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "CorePlayerController.h"
#include "CorePawn.h"
#include "Application/UI/Timebar/Public/Timebar.h"


ACoreGameMode::ACoreGameMode()
{
	PlayerControllerClass = ACorePlayerController::StaticClass();
	DefaultPawnClass = ACorePawn::StaticClass();
}

ACoreGameMode::~ACoreGameMode()
{
	
}

void ACoreGameMode::BeginDestroy()
{
	UTimebarPlayer::DestroyTimebarPlayer();
	if (FModuleManager::Get().IsModuleLoaded(TEXT("Timebar")))
	{
		FTimebar* Module = FModuleManager::Get().GetModulePtr<FTimebar>("Timebar");
		if (Module)
		{
			Module->Controller->Shutdown();
		}
	}

	Super::BeginDestroy();
}

