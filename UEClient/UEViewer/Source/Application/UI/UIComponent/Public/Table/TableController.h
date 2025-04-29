// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TableController.generated.h"

USTRUCT()
struct FTableManagerWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	UTableManager* Manager;

	FTableManagerWrapper()
		: Manager(nullptr)
	{}
};


UCLASS()
class UICOMPONENT_API UTableController : public UObject
{
	GENERATED_BODY()

	// construct
public:
	UTableController();
	virtual ~UTableController();
	virtual void BeginDestroy() override;
	void Shutdown();


	// Manager
public:
	UFUNCTION(BlueprintCallable)
	UTableManager* GetTableManager(FName InManagerName);

	TMap<FName, FTableManagerWrapper> ManagerMap;
};
