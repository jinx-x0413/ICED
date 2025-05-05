// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableRow.h"
#include "TableDependency.h"


// construct
void UTableRow::InitializeTableItem(FTableRowData InData)
{
	TargetRowData = InData;
	if (!OnRowFocused.IsAlreadyBound(this, &UTableRow::SetActivated))
	{
		OnRowFocused.AddDynamic(this, &UTableRow::SetActivated);
	}
}




// Feature
FString UTableRow::GetFieldValue(int InColumnIndex)
{
	return FString();
}

FTableRowData UTableRow::GetCurrentRowData()
{
	ExecGetCurrentRowData();

	FTableRowData TempRowData;
	TempRowData.RowIndex = TargetRowData.RowIndex;
	TempRowData.RowName = TargetRowData.RowName;

	for (auto& Field : Fields)
	{
		FTableFieldData TempFieldData = Field->GetFieldData();
		TempRowData.Fields.Add(TempFieldData);
	}

	return TempRowData;
}

void UTableRow::SetFieldValues(const TArray<FString>& NewValueArray)
{
}

void UTableRow::AddField(UTableField* InField)
{
	if (IsValid(InField))
	{
		Fields.Add(InField);

		if (!OnFieldAdded.IsAlreadyBound(InField, &UTableField::InitializeValue))
		{
			OnFieldAdded.AddDynamic(InField, &UTableField::InitializeValue);
		}

		if (!InField->OnFieldFocused.IsAlreadyBound(this, &UTableRow::HandleFieldFocus))
		{
			InField->OnFieldFocused.AddDynamic(this, &UTableRow::HandleFieldFocus);
		}
		InField->ParentTableItem = this;
		OnFieldAdded.Broadcast(InField);
	}

}




// Interface : button
void UTableRow::SetActivated(UTableRow* InWidget, bool bIsActivated)
{

	ExecSetActivated(bIsActivated);
	bIsButtonActivated = bIsActivated;

}

void UTableRow::Reset(bool bIsActivated)
{
	if (!bIsActivated)
	{
		FocusedFields.Empty();
		bIsButtonActivated = false;
		ExecReset();
	}
}

void UTableRow::HandleFieldFocus(UTableField* InWidget, bool bIsActivated)
{
	if (!FocusedFields.Contains(InWidget) && bIsActivated)
	{
		FocusedFields.Add(Cast<UTableField>(InWidget));
	}
	else if (!bIsActivated)
	{
		FocusedFields.Remove(Cast<UTableField>(InWidget));
	}

	if (FocusedFields.Num() == 0)
	{
		SetActivated(nullptr, false);
	}
	else
	{
		SetActivated(nullptr, true);
	}


}
