// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetContentController.h"
#include "AssetContentDependency.h"

// construct
UAssetContentController::UAssetContentController()
{
}

UAssetContentController::~UAssetContentController()
{
	if (IsValid(this) && IsRooted())
	{
		RemoveFromRoot();
		MarkAsGarbage();
	}
}

UInteractionBase* UAssetContentController::CreateInteraction(TSubclassOf<UInteractionBase> InInteractionClass)
{
	return nullptr;
}
