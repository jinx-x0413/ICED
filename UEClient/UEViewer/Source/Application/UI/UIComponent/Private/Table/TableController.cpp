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
	RemoveFromRoot();
	MarkAsGarbage();

	Super::BeginDestroy();
}

void UTableController::Shutdown()
{
	if (!ManagerMap.IsEmpty())
	{
		ManagerMap.Empty();
	}
}




// Manager
UTableManager* UTableController::GetTableManager(FName InManagerName)
{
	UTableManager* CurrentManager = nullptr;


	if (ManagerMap.Contains(InManagerName)) // get
	{
		//uint32 Hash = GetTypeHash(InManagerName);
		CurrentManager = ManagerMap[InManagerName].Manager;
	}
	else // create
	{
		CurrentManager = NewObject<UTableManager>(GetTransientPackage());
		CurrentManager->AddToRoot();
		FTableManagerWrapper NewStruct;
		NewStruct.Manager = CurrentManager;
		ManagerMap.Add(InManagerName, NewStruct);
	}

	return CurrentManager;
}
