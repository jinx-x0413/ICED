// Fill out your copyright notice in the Description page of Project Settings.


#include "Template/TemplateHandler.h"
#include "TemplateBuildDependency.h"

// construct / singleton
UTemplateHandler* UTemplateHandler::Instance = nullptr;

UTemplateHandler* UTemplateHandler::GetTemplateHandler()
{
	if (!Instance)
	{
		Instance = NewObject<UTemplateHandler>();
		Instance->AddToRoot();
	}
	return Instance;
}

UTemplateHandler::UTemplateHandler()
	: CurrentTemplateBuildStep(ETemplateBuildStep::NONE)
	, CurrentTemplateType(ETemplateType::NONE)
	, CurrentTemplateLayoutType(ETemplateLayoutType::NONE)
{
}

UTemplateHandler::~UTemplateHandler()
{
	if (IsValid(Instance) && Instance->IsRooted())
	{
		Instance->RemoveFromRoot();
		Instance->MarkAsGarbage();
		Instance = nullptr;
	}
}

void UTemplateHandler::Initialize()
{
	CurrentTemplateBuildStep = ETemplateBuildStep::NONE;
	CurrentTemplateType = ETemplateType::NONE;
	CurrentTemplateLayoutType = ETemplateLayoutType::NONE;
	// TODO : Initialize CurrentData
}




// feature
void UTemplateHandler::StartTemplateBuildStep()
{
	CurrentTemplateBuildStep = ETemplateBuildStep::NONE;
	HandleTemplateBuildStep();
	OnTemplateBuildStarted.Broadcast();
}

void UTemplateHandler::FinishTemplateBuildStep()
{
	ResetTemplateBuildStep();
	OnTemplateBuildFinished.Broadcast();
}

void UTemplateHandler::HandleTemplateBuildStep()
{
	switch (CurrentTemplateBuildStep)
	{
	case ETemplateBuildStep::NONE :
		CurrentTemplateBuildStep = ETemplateBuildStep::SELECT_TEMPLATETYPE;
		break;
	case ETemplateBuildStep::SELECT_TEMPLATETYPE :
		CurrentTemplateBuildStep = ETemplateBuildStep::SELECT_TEMPLATELAYOUTTYPE;
		break;
	case ETemplateBuildStep::SELECT_TEMPLATELAYOUTTYPE :
		CurrentTemplateBuildStep = ETemplateBuildStep::SELECT_TEMPLATEDATA;
		break;
	case ETemplateBuildStep::SELECT_TEMPLATEDATA :
		CurrentTemplateBuildStep = ETemplateBuildStep::NONE;
		FinishTemplateBuildStep();
		break;
	default:
		CurrentTemplateBuildStep = ETemplateBuildStep::NONE;
		break;
	}

	OnTemplateBuildStepSet.Broadcast(CurrentTemplateBuildStep);
}

void UTemplateHandler::SetTemplateBuildStep(ETemplateBuildStep InTemplateBuildStep)
{
	CurrentTemplateBuildStep = InTemplateBuildStep;
	OnTemplateBuildStepSet.Broadcast(CurrentTemplateBuildStep);
}

void UTemplateHandler::ResetTemplateBuildStep()
{
	CurrentTemplateBuildStep = ETemplateBuildStep::NONE;
	OnTemplateBuildStepSet.Broadcast(CurrentTemplateBuildStep);
	OnTemplateBuildReset.Broadcast();
}

void UTemplateHandler::ApplyTemplateType(ETemplateType InTemplateType)
{
	CurrentTemplateType = InTemplateType;
	HandleTemplateBuildStep();
	OnTemplateTypeApplied.Broadcast(InTemplateType);
}

void UTemplateHandler::ApplyTemplateLayoutType(ETemplateLayoutType InTemplateLayoutType)
{
	CurrentTemplateLayoutType = InTemplateLayoutType;
	HandleTemplateBuildStep();
	OnTemplateLayoutTypeApplied.Broadcast(InTemplateLayoutType);
}

void UTemplateHandler::ApplyTemplateData()
{
	// TODO : set current Template Data
	HandleTemplateBuildStep();
	OnTemplateDataApplied.Broadcast();
}
