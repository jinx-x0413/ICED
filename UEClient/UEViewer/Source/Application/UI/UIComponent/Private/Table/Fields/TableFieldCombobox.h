// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/TableField.h"
#include "TableFieldCombobox.generated.h"

/**
 * 
 */
UCLASS()
class UTableFieldCombobox : public UTableField
{
	GENERATED_BODY()
	
	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintImplementableEvent)
	void ExecConstruct();


	// feature
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FName> TransformDirectionArray;

	
};
