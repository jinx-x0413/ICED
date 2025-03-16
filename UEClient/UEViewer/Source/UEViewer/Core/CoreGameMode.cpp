// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameMode.h"
#include "CorePlayerController.h"
#include "CorePawn.h"


ACoreGameMode::ACoreGameMode()
{
	PlayerControllerClass = ACorePlayerController::StaticClass();
	DefaultPawnClass = ACorePawn::StaticClass();

}
