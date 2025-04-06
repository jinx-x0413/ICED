// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommandBase.generated.h"


USTRUCT(BlueprintType)
struct FCommandData
{
	GENERATED_BODY()

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	FTransform OldTransform;

	UPROPERTY()
	FTransform NewTransform;

	FCommandData() 
		: TargetActor()
		, OldTransform(FTransform())
		, NewTransform(FTransform())
	{
	}
};

UCLASS()
class UCommandBase : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UFUNCTION()
	virtual void Initialize(FCommandData& InData);

	UPROPERTY()
	FCommandData TargetData;


	// feature
public:
	UFUNCTION()
	virtual void Execute();

	UFUNCTION()
	virtual void Undo();
};
