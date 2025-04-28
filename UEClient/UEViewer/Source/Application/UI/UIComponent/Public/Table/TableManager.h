// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TableManager.generated.h"

class UTableContainer;
class UTableRow;
class UTableField;
class UTableFieldText;
class UInteractionBase;

// struct
USTRUCT(BlueprintType)
struct FTableFieldData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	int32 FieldIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	TSubclassOf<class UTableField> FieldClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	FString FieldName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	FString FieldValue;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	TSubclassOf<UInteractionBase> InteractionClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	UTableManager* TargetManager;

	FTableFieldData()
		: FieldIndex(0)
		, FieldClass()
		, FieldName(TEXT(""))
		, FieldValue(TEXT(""))
		, InteractionClass()
		, TargetManager(nullptr)
	{}
};


USTRUCT(BlueprintType)
struct FTableRowData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	int32 RowIndex;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	FString RowName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	TArray<FTableFieldData> Fields;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	TArray<FTableFieldData> FieldsBackward;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	UTableManager* TargetManager;

	FTableRowData()
		: RowIndex(0)
		, RowName(TEXT(""))
		, Fields()
		, FieldsBackward()
		, TargetManager(nullptr)
	{}
};


USTRUCT(BlueprintType)
struct FTableData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	FString TableName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Table")
	TArray<FTableRowData> Rows;

	FTableData()
		: TableName(TEXT(""))
		, Rows()
	{}
};


// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTableCreated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTableDeleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTableUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAllFieldsActivated, int32, FieldIndex);


UCLASS(BlueprintType)
class UICOMPONENT_API UTableManager : public UObject
{
	GENERATED_BODY()
	
	// construct singleton
public:
	UTableManager();
	virtual ~UTableManager();
	void BeginDestroy();
	/*UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	static UTableManager* GetTableManager();*/

	UFUNCTION(BlueprintCallable)
	void Initialize(FTableData InData);
	UFUNCTION()
	void Shutdown();
//
//private:
//	static UTableManager* Instance;



	// Feature
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTableContainer* TargetTable;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTableData TableData;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTableCreated OnTableCreated;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTableDeleted OnTableDeleted;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTableUpdated OnTableUpdated;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnAllFieldsActivated OnAllFieldsActivated;


	UFUNCTION(BlueprintCallable)
	virtual void CreateTable(TSubclassOf<UTableContainer> InContainerClass, TSubclassOf<UTableRow> InRowClass, FVector2D InPosition);
	UFUNCTION(BlueprintCallable)
	virtual void DeleteTable();

	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual FTableData GetTableData();
	UFUNCTION(BlueprintCallable)
	virtual void UpdateTable();
};
