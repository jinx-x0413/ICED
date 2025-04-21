// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableField.h"
#include "Table/TableDependency.h"

void UTableField::NativeConstruct()
{
    Super::NativeConstruct();
}

void UTableField::NativeDestruct()
{
    Super::NativeDestruct();

   /* if (ParentTableItem && IsValid(ParentTableItem->TargetTable))
    {
        if (ParentTableItem->TargetTable->TargetManager->OnAllFieldsActivated.IsAlreadyBound(this, &UTableField::SetFieldActivated))
        {
            ParentTableItem->TargetTable->TargetManager->OnAllFieldsActivated.RemoveDynamic(this, &UTableField::SetFieldActivated);
        }
    }*/

}

void UTableField::InitializeTableField(FTableFieldData InData)
{
    TargetFieldData = InData;

    if (ParentTableItem && IsValid(ParentTableItem->TargetTable))
    {
        if (!ParentTableItem->TargetTable->TargetManager->OnAllFieldsActivated.IsAlreadyBound(this, &UTableField::SetFieldActivated))
        {
            ParentTableItem->TargetTable->TargetManager->OnAllFieldsActivated.AddDynamic(this, &UTableField::SetFieldActivated);
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

void UTableField::SetFieldActivated(int32 InFieldIndex)
{
    if (InFieldIndex == TargetFieldData.FieldIndex)
    {

        ExecSetFieldActivated();
    }
    
}
