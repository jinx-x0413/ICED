// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableManager.h"
#include "Table/TableDependency.h"


// singleton
UTableManager* UTableManager::Instance = nullptr;
UTableManager::UTableManager()
{
}

UTableManager::~UTableManager()
{
	if (IsValid(Instance) && Instance->IsRooted())
	{
		Instance->Shutdown();
		Instance->RemoveFromRoot();
		Instance->MarkAsGarbage();
		Instance = nullptr;
	}
}

UTableManager* UTableManager::GetTableManager()
{
	if (!Instance)
	{
		Instance = NewObject<UTableManager>();
		Instance->AddToRoot();
	}

	return Instance;
}



// construct
void UTableManager::Initialize(FTableData InData)
{
	TableData = InData;
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

	//TargetTable = NewObject<UTableContainer>(this, InContainerClass);

	APlayerController* PlayerController = GWorld->GetFirstPlayerController();
	TargetTable = CreateWidget<UTableContainer>(PlayerController, InContainerClass);
	TargetTable->InitializeTable(this);
	TargetTable->AddToViewport();
	TargetTable->SetPositionInViewport(UViewportHelper::AdjustedPosition(InPosition), false);



	for (int i = 0; i < TableData.Rows.Num(); i++)
	{
		TWeakObjectPtr<UTableRow> NewRow = CreateWidget<UTableRow>(PlayerController, InRowClass);
		NewRow->InitializeTableItem(TableData.Rows[i]);
		TargetTable->AddRow(NewRow.Get());
		for (int j = 0; j < TableData.Rows[i].Fields.Num(); j++)
		{
			TWeakObjectPtr<UTableField> NewField = CreateWidget<UTableField>(PlayerController, TableData.Rows[i].Fields[j].FieldClass);
			NewField->InitializeTableField(TableData.Rows[i].Fields[j]);
			TableData.Rows[i].Fields[j].FieldIndex = j;
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




