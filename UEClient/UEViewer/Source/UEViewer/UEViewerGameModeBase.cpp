// Fill out your copyright notice in the Description page of Project Settings.


#include "UEViewerGameModeBase.h"
#include "UEViewerPlayerController.h"

AUEViewerGameModeBase::AUEViewerGameModeBase()
{
	PlayerControllerClass = AUEViewerPlayerController::StaticClass();
}

AUEViewerGameModeBase::~AUEViewerGameModeBase()
{
}
