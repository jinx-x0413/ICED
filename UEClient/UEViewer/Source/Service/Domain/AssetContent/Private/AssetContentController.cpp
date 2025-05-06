// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetContentController.h"
#include "AssetContentDependency.h"

// construct
UAssetContentController::UAssetContentController()
{
}

UAssetContentController::~UAssetContentController()
{
	
}

void UAssetContentController::BeginDestroy()
{
    Shutdown();

    Super::BeginDestroy();

}

void UAssetContentController::Shutdown()
{
    if (IsValid(TemplateBuilder) && TemplateBuilder->IsRooted())
    {
        TemplateBuilder->RemoveFromRoot();
        TemplateBuilder->MarkAsGarbage();
        TemplateBuilder = nullptr;
    }

    TrackHeaderWidgetClass = nullptr;
    TrackWidgetClass = nullptr;
    TrackSceneCaptureWidgetClass = nullptr;
}

UInteractionBase* UAssetContentController::CreateInteraction(TSubclassOf<UInteractionBase> InInteractionClass, FInteractionData InInteractionData, UTrack* InTrack, float InStartTime, float InEndTime)
{
    if (InInteractionClass->IsChildOf(UInteractionBase::StaticClass()))
    {
        UInteractionBase* NewInteraction = NewObject<UInteractionBase>(GetTransientPackage(), InInteractionClass);
        InInteractionData.TargetClass = InInteractionClass;
        NewInteraction->Initialize(InInteractionData);
        NewInteraction->TargetTrack = InTrack;
        NewInteraction->StartTime = InStartTime;
        NewInteraction->EndTime = InEndTime;
        NewInteraction->ClipLength = InEndTime - InStartTime;
        NewInteraction->Name = InInteractionData.Name.ToString();
        NewInteraction->AddToRoot();

        UTimebarPlayer::GetTimebarPlayer()->OnClipCreated.Broadcast(InTrack, NewInteraction);
        return NewInteraction;
    }

    return nullptr;
}

void UAssetContentController::BuildTemplate(ETemplateType InTemplateType, AActor* InActor, FTableData InTableData)
{
    if (!IsValid(TemplateBuilder))
    {
        TemplateBuilder = NewObject<UTemplateBuilder>(GetTransientPackage());
        TemplateBuilder->AddToRoot();
    }
    TemplateBuilder->SetTargetActor(InActor);
    TemplateBuilder->SetController(this);
    TemplateBuilder->Build(InTemplateType, InTableData);
}

