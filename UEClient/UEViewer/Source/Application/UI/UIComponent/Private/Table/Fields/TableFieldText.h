// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Table/TableField.h"
#include "TableFieldText.generated.h"

/**
 * 
 */
UCLASS()
class UTableFieldText : public UTableField
{
	GENERATED_BODY()


public:
	virtual FString GetFieldValue()
	{
		return TargetFieldData.FieldValue;
	}

	virtual void SetFieldValue(const FString& Value)
	{
		TargetFieldData.FieldValue = Value;
	}
};
