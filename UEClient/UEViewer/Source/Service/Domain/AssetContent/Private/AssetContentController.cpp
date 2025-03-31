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

UInteractionBase* UAssetContentController::CreateInteraction(FInteractionData InInteractionData, UTrack* InTrack, float InStartTime, float InEndTime)
{
    if (InInteractionData.TargetClass->IsChildOf(UInteractionBase::StaticClass()))
    {
        UInteractionBase* NewInteraction = NewObject<UInteractionBase>(this, InInteractionData.TargetClass);
        //InInteractionData.TargetClass = InInteractionData.TargetClass;
        NewInteraction->Initialize(InInteractionData);
        NewInteraction->StartTime = InStartTime;
        NewInteraction->EndTime = InEndTime;
        NewInteraction->ClipLength = InEndTime - InStartTime;
        NewInteraction->Name = InInteractionData.Name.ToString();
        NewInteraction->AddToRoot();

        return NewInteraction;
    }

    return nullptr;
}

