// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableField.h"
#include "Table/TableDependency.h"

void UTableField::InitializeTableField(FTableFieldData InData)
{
    TargetFieldData = InData;

    if (IsValid(TargetFieldData.TargetTableManager))
    {
        if (!TargetFieldData.TargetTableManager->OnAllFieldsChecked.IsAlreadyBound(this, &UTableField::SetFieldChecked))
        {
            TargetFieldData.TargetTableManager->OnAllFieldsChecked.AddDynamic(this, &UTableField::SetFieldChecked);
        }
    
        if (!TargetFieldData.TargetTableManager->OnAllFieldsSelected.IsAlreadyBound(this, &UTableField::SetFieldSelected))
        {
            TargetFieldData.TargetTableManager->OnAllFieldsSelected.AddDynamic(this, &UTableField::SetFieldSelected);
        }
    }

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

void UTableField::SetFieldChecked(int32 InFieldIndex, bool bIsChecked)
{
    if (TargetFieldData.FieldIndex == InFieldIndex)
    {
        ExecSetFieldChecked(bIsChecked);
    }
}


void UTableField::SetFieldSelected(int32 InFieldIndex, int32 InSelectedIndex)
{
    if (TargetFieldData.FieldIndex == InFieldIndex)
    {
        ExecSetFieldSelected(InSelectedIndex);
    }
}

