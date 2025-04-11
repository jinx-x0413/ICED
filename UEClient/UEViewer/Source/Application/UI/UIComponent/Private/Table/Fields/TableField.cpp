// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableField.h"
#include "Table/TableDependency.h"

void UTableField::InitializeTableField(FTableFieldData InData)
{
    TargetFieldData = InData;

}

void UTableField::InitializeValue(UTableField* InField)
{
    if (IsValid(InField))
    {
        ExecInitializeTableField();
    }
}

FTableFieldData UTableField::GetFieldData()
{
    ExecGetFieldData();

    return TargetFieldData;
}

void UTableField::SetFieldValue(const FString& Value)
{
    TargetFieldData.FieldValue = Value;
}
