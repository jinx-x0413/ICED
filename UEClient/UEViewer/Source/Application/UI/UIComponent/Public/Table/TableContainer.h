// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TableContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTableRowAdded, UTableRow*, InRow);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTableRowRemoved, UTableRow*, InRow);

class UScrollBox;

UCLASS()
class UICOMPONENT_API UTableContainer : public UDroppableWidget
{
	GENERATED_BODY()

	// construct
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UScrollBox* TargetScrollBox;

	UFUNCTION()
	virtual void InitializeTable(UTableManager* InManager);
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTableManager* TargetManager;


	// Feature

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TWeakObjectPtr<UTableRow> HeaderRow;
	UPROPERTY()
	TArray < TWeakObjectPtr<UTableRow >> Rows;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTableRowAdded OnTableRowAdded;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnTableRowRemoved OnTableRowRemoved;

	UFUNCTION(BlueprintCallable)
	virtual FString GetRowValue(int RowIndex, int ColumnIndex);
	UFUNCTION(BlueprintCallable)
	virtual void SetRowValue(int RowIndex, int ColumnIndex, const FString& NewValue);
	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual void AddRow(UTableRow* InRowWidget);
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	virtual void RemoveRow(UTableRow* InRowWidget);


	UFUNCTION()
	void SetHeader();
	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetHeader(FTableRowData InRowData);



	// dragdrop overriding
public:
	virtual bool NativeOnDrop(
		const FGeometry& InGeometry
		, const FDragDropEvent& InDragDropEvent
		, UDragDropOperation* InOperation) override;
};
