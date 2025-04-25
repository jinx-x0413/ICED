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
}

void UTableManager::BeginDestroy()
{
	Super::BeginDestroy();

	if (IsValid(Instance) && Instance->IsRooted())
	{
		Instance->Shutdown();
		Instance->RemoveFromRoot();
		Instance->ConditionalBeginDestroy();
		Instance = nullptr;
	}
}

UTableManager* UTableManager::GetTableManager()
{
	if (!Instance)
	{
		Instance = NewObject<UTableManager>(GetTransientPackage());
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

	// 1. Create Container
	APlayerController* PlayerController = GWorld->GetFirstPlayerController();
	TargetTable = CreateWidget<UTableContainer>(PlayerController, InContainerClass);
	TargetTable->InitializeTable(this);
	TargetTable->AddToViewport();
	TargetTable->SetPositionInViewport(UViewportHelper::AdjustedPosition(InPosition), false);


	// 2. Create Row
	for (int i = 0; i < TableData.Rows.Num(); i++)
	{
		//TWeakObjectPtr<UTableRow> NewRow = CreateWidget<UTableRow>(PlayerController, InRowClass);
		UTableRow* NewRow = CreateWidget<UTableRow>(PlayerController, InRowClass);
		TargetTable->AddRow(NewRow);
		
		// 3. Field
		for (int j = 0; j < TableData.Rows[i].Fields.Num(); j++)
		{
			// 3-1. Create Fields
			//TWeakObjectPtr<UTableField> NewField = CreateWidget<UTableField>(PlayerController, TableData.Rows[i].Fields[j].FieldClass);
			UTableField* NewField = CreateWidget<UTableField>(PlayerController, TableData.Rows[i].Fields[j].FieldClass);
			
			// set row
			//NewRow->AddField(NewField.Get());
			NewRow->AddField(NewField);
			NewRow->TargetTable = TargetTable;
			
			// set field
			NewField->InitializeTableField(TableData.Rows[i].Fields[j]);
			TableData.Rows[i].Fields[j].FieldIndex = j;
			
			// initialize item
			NewRow->InitializeTableItem(TableData.Rows[i]);


			// 3-2. Create FieldsBackward
			TableData.Rows[i].FieldsBackward[j].FieldIndex = j;
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

	// TODO : get fieldsBakward data from datatable
	/*for (int i = 0; i < TableData.Rows.Num(); i++)
	{
		for (int j = 0; j < TableData.Rows[i].FieldsBackward.Num(); j++)
		{
			CurrentTableData.Rows[i].FieldsBackward = TableData.Rows[i].FieldsBackward;
		}
	}*/

	return CurrentTableData;
}

void UTableManager::UpdateTable()
{
	TableData = GetTableData();
}




