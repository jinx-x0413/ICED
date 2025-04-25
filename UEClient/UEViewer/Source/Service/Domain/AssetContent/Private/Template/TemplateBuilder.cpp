// Fill out your copyright notice in the Description page of Project Settings.


#include "Template/TemplateBuilder.h"
#include "TemplateBuildDependency.h"

void UTemplateBuilder::SetController(UAssetContentController* InAssetActorController)
{
	Controller = InAssetActorController;
}

void UTemplateBuilder::SetTargetActor(AActor* InActor)
{
	TargetActor = Cast<AGltfAssetActor>(InActor);
}

TScriptInterface<ITemplateInterface> UTemplateBuilder::SetCurrentTemplate(ETemplateType InTemplateType)
{
	
	switch (InTemplateType)
	{
	case ETemplateType::NONE:
		break;
		return nullptr;
	case ETemplateType::ASSEMBLY:
		return NewObject<UAssemblyBuilder>(GetTransientPackage());
		break;
	case ETemplateType::COMPONENT:
		return NewObject<UComponentBuilder>(GetTransientPackage());
		break;
	default:
		return nullptr;
		break;
	}
		
	return nullptr;
	
}


void UTemplateBuilder::Build(ETemplateType InTemplateType, FTableData InTableData)
{
	CurrentTemplate = SetCurrentTemplate(InTemplateType);
	CurrentTemplate->TargetActor = TargetActor;
	CurrentTemplate->Controller = Controller;
	CurrentTemplate->TargetTableData = InTableData;
	CurrentTemplate->Build();
}

