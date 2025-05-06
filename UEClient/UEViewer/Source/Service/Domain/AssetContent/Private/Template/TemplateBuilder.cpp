// Fill out your copyright notice in the Description page of Project Settings.


#include "Template/TemplateBuilder.h"
#include "TemplateBuildDependency.h"

// construct
UTemplateBuilder::UTemplateBuilder()
{
}

UTemplateBuilder::~UTemplateBuilder()
{
}

void UTemplateBuilder::BeginDestroy()
{
	Shutdown();
	Super::BeginDestroy();
}

void UTemplateBuilder::Shutdown()
{
	if (TargetActor)
	{
		TargetActor = nullptr;
	}

	//if (IsValid(CurrentTemplate.GetObject()) && CurrentTemplate.GetObject()->IsRooted())
	//{
	//	CurrentTemplate.GetObject()->RemoveFromRoot();
	//	CurrentTemplate.GetObject()->MarkAsGarbage();
	//}
}





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
	UObject* TemplateObject = nullptr;
	switch (InTemplateType)
	{
	case ETemplateType::NONE:
		break;
	case ETemplateType::ASSEMBLY:
		TemplateObject = NewObject<UAssemblyBuilder>(GetTransientPackage());
		break;
	case ETemplateType::DISASSEMBLY:
		TemplateObject = NewObject<UDisassemblyBuilder>(GetTransientPackage());
		break;
	default:
		break;
	}

	TScriptInterface<ITemplateInterface> TemplateInterface;
	if (TemplateObject && TemplateObject->GetClass()->ImplementsInterface(UTemplateInterface::StaticClass()))
	{
		TemplateInterface.SetObject(TemplateObject);
		TemplateInterface.SetInterface(Cast<ITemplateInterface>(TemplateObject));
		return TemplateInterface;
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

