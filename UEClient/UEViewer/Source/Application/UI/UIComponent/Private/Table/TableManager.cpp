// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableManager.h"
#include "Table/TableDependency.h"


// singleton
//UTableManager* UTableManager::Instance = nullptr;
UTableManager::UTableManager()
{
}

UTableManager::~UTableManager()
{

}
void UTableManager::BeginDestroy()
{
	Shutdown();

	Super::BeginDestroy();
}



// construct
void UTableManager::Initialize(FTableData InData)
{
	FString CurrentTableName = InData.TableName;
	TableData = InData;
	TableData.TableName = CurrentTableName;
}

void UTableManager::Shutdown()
{
	
	if (IsValid(TargetTable))
	{
		TargetTable->RemoveFromParent();
	}

}




// feature
void UTableManager::CreateTable(TSubclassOf<UTableContainer> InContainerClass, TSubclassOf<UTableRow> InRowClass, FVector2D InPosition)
{
	if (TargetTable)
	{
		return;
	}

	
	APlayerController* PlayerController = GWorld->GetFirstPlayerController();
	TargetTable = CreateWidget<UTableContainer>(PlayerController, InContainerClass);
	TargetTable->InitializeTable(this);
	TargetTable->AddToViewport();
	TargetTable->SetPositionInViewport(UViewportHelper::AdjustedPosition(InPosition), false);



	for (int i = 0; i < TableData.Rows.Num(); i++)
	{
		TWeakObjectPtr<UTableRow> NewRow = CreateWidget<UTableRow>(PlayerController, InRowClass);
		TableData.Rows[i].TargetTableManager = this;
		NewRow->InitializeTableItem(TableData.Rows[i]);
		TargetTable->AddRow(NewRow.Get());
		for (int j = 0; j < TableData.Rows[i].Fields.Num(); j++)
		{
			TWeakObjectPtr<UTableField> NewField = CreateWidget<UTableField>(PlayerController, TableData.Rows[i].Fields[j].FieldClass);
			TableData.Rows[i].Fields[j].FieldIndex = j;
			TableData.Rows[i].Fields[j].TargetTableManager = this;
			NewField->InitializeTableField(TableData.Rows[i].Fields[j]);
			NewRow->AddField(NewField.Get());
		}
	}

	OnTableCreated.Broadcast();
}

void UTableManager::DeleteTable()
{
}

FTableData UTableManager::GetTableData()
{
	FTableData CurrentTableData;
	CurrentTableData.TableName = TableData.TableName;

	for (auto& Row : TargetTable->Rows)
	{
		FTableRowData CurrentRowData;
		CurrentRowData = Row->GetCurrentRowData();
		CurrentTableData.Rows.Add(CurrentRowData);
	}

	return CurrentTableData;
}

void UTableManager::UpdateTable()
{
	TableData = GetTableData();
}

void UTableManager::CheckAllFields(int32 InFieldIndex, bool bIsChecked)
{
	OnAllFieldsChecked.Broadcast(InFieldIndex, bIsChecked);
}

void UTableManager::SelectAllFields(int32 InFieldIndex, int32 InSelectedOption)
{
	OnAllFieldsSelected.Broadcast(InFieldIndex, InSelectedOption);
}




