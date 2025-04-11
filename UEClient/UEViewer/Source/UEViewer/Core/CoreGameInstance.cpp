// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreGameInstance.h"
#include "Infra/Log/Public/LogManager.h"

void UCoreGameInstance::Init()
{
	Super::Init();

	ULogManager::GetLogManager()->ParseAuthToken();
}
