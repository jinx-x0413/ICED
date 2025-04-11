// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TemplateHandler.generated.h"

UENUM(BlueprintType)
enum class ETemplateBuildStep : uint8
{
	NONE						UMETA(DisplayName = "None"),
	SELECT_TEMPLATETYPE			UMETA(DisplayName = "Select Template Type"),
	SELECT_TEMPLATELAYOUTTYPE	UMETA(DisplayName = "Select Template Layout Type"),
	SELECT_TEMPLATEDATA			UMETA(DisplayName = "Select Template Data")
};

UENUM(BlueprintType)
enum class ETemplateLayoutType : uint8
{
	NONE				UMETA(DisplayName = "None"),
	ASSEMBLY_LAYOUT		UMETA(DisplayName = "Assembly Layout")

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTemplateBuildStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTemplateBuildFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTemplateBuildReset);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTemplateBuildStepSet, ETemplateBuildStep, InTemplateBuildStep);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTemplateTypeApplied, ETemplateType, InTemplateType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTemplateLayoutTypeApplied, ETemplateLayoutType, InTemplateLayoutType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTemplateDataApplied);


UCLASS(BlueprintType)
class ASSETCONTENT_API UTemplateHandler : public UObject
{
	GENERATED_BODY()
	
	// construct / singleton
public:
	static UTemplateHandler* Instance;
	UFUNCTION(BlueprintCallable)
	static UTemplateHandler* GetTemplateHandler();
	UTemplateHandler();
	virtual ~UTemplateHandler();
	virtual void Initialize();


	// feature
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETemplateBuildStep CurrentTemplateBuildStep;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETemplateType CurrentTemplateType;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETemplateLayoutType CurrentTemplateLayoutType;
	// TemplateData

	UFUNCTION(BlueprintCallable)
	virtual void StartTemplateBuildStep();

	UFUNCTION(BlueprintCallable)
	virtual void FinishTemplateBuildStep();

	UFUNCTION()
	virtual void HandleTemplateBuildStep();

	UFUNCTION(BlueprintCallable)
	virtual void SetTemplateBuildStep(ETemplateBuildStep InTemplateBuildStep);

	UFUNCTION(BlueprintCallable)
	virtual void ResetTemplateBuildStep();

	UFUNCTION(BlueprintCallable)
	virtual void ApplyTemplateType(ETemplateType InTemplateType);

	UFUNCTION(BlueprintCallable)
	virtual void ApplyTemplateLayoutType(ETemplateLayoutType InTemplateLayoutType);

	UFUNCTION(BlueprintCallable)
	virtual void ApplyTemplateData();



	// delegate
public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTemplateBuildStarted OnTemplateBuildStarted;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTemplateBuildStepSet OnTemplateBuildStepSet;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTemplateBuildFinished OnTemplateBuildFinished;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTemplateBuildReset OnTemplateBuildReset;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTemplateTypeApplied OnTemplateTypeApplied;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTemplateLayoutTypeApplied OnTemplateLayoutTypeApplied;
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTemplateDataApplied OnTemplateDataApplied;

};
