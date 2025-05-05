// Fill out your copyright notice in the Description page of Project Settings.


#include "Table/TableController.h"
#include "TableDependency.h"

UTableController::UTableController()
{
}

UTableController::~UTableController()
{
}

void UTableController::BeginDestroy()
{
	Shutdown();

	Super::BeginDestroy();
}




void UTableController::Shutdown()
{
	if (!ManagerMap.IsEmpty())
	{
		for (auto& Wrapper : ManagerMap)
		{
			if (UTableManager* Manager = Wrapper.Value.Manager)
			{
				if (IsValid(Manager) && Manager->IsRooted())
				{
					Manager->RemoveFromRoot();
					Manager->MarkAsGarbage();
				}
			}

			Wrapper.Value.Manager = nullptr;
		}

		ManagerMap.Empty();
	}
}

UTableManager* UTableController::GetTableManager(FName InManagerName)
{
	UTableManager* CurrentManager = nullptr;

	if (!ManagerMap.Contains(InManagerName)) // create
	{
		//UE_LOG(LogTemp, Warning, TEXT("No Manager Name in Map at UDragDropController::GetDragDropManager"));
		CurrentManager = NewObject<UTableManager>();

		CurrentManager->TableData.TableName = InManagerName.ToString();
		CurrentManager->AddToRoot();
		ManagerMap.Add(InManagerName);
		ManagerMap[InManagerName].Manager = CurrentManager;
	}
	else // get
	{
		uint32 KeyHash = GetTypeHash(InManagerName);
		CurrentManager = ManagerMap.FindByHash(KeyHash, InManagerName)->Manager;
		if (!IsValid(CurrentManager))
		{
			UE_LOG(LogTemp, Warning, TEXT("No Manager in Map at UTableController::GetTableManager"));
			return nullptr;
		}
	}

	return CurrentManager;
}
