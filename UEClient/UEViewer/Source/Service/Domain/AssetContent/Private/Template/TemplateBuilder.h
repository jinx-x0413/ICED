// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TemplateBuilder.generated.h"

class ITemplateInterface;
class AGltfAssetActor;
class UAssetContentController;
struct FTableData;

UENUM(BlueprintType)
enum class ETemplateType : uint8
{
	NONE			UMETA(DisplayName = "None"),
	FREE			UMETA(DisplayName = "Free"),
	PARTINFO		UMETA(DisplayName = "PartInfo"),
	ASSEMBLY		UMETA(DisplayName = "Assembly"),
	DISASSEMBLY		UMETA(DisplayName = "Disassembly")
};


UCLASS()
class UTemplateBuilder : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UTemplateBuilder();
	virtual ~UTemplateBuilder();
	virtual void BeginDestroy() override;
	void Shutdown();






	// feature
public:
	UAssetContentController* Controller;
	void SetController(UAssetContentController* InAssetActorController);

	AGltfAssetActor* TargetActor;
	void SetTargetActor(AActor* InActor);

	TScriptInterface<ITemplateInterface> CurrentTemplate;
	TScriptInterface<ITemplateInterface> SetCurrentTemplate(ETemplateType InTemplateType);


	virtual void Build(ETemplateType InTemplateType, FTableData InTableData);


};
