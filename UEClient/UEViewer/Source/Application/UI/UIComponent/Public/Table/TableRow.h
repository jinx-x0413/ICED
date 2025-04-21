// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TableManager.h"

#include "TableRow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFieldAdded, UTableField*, InTableField);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFieldRemoved, UTableField*, InTableField);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRowFocused, UTableRow*, InWidget, bool, bIsActivated);

struct FTableRowData;


UCLASS()
class UICOMPONENT_API UTableRow : public UDraggableWidget
{
	GENERATED_BODY()

	// construct
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTableRowData TargetRowData;
	UFUNCTION()
	void InitializeTableItem(FTableRowData InData);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTableContainer* TargetTable;



	// Feature
	//// Fields

public:
	UPROPERTY()
	TArray<UTableField*> Fields;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnRowFocused OnRowFocused;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnFieldAdded OnFieldAdded;
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnFieldRemoved OnFieldRemoved;

	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual FString GetFieldValue(int InColumnIndex); // 특정 필드값 가져오기

	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual FTableRowData GetCurrentRowData();
	UFUNCTION(BlueprintImplementableEvent, meta = (AllowPrivateAccess = true))
	void ExecGetCurrentRowData();

	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual void SetFieldValues(const TArray<FString>& NewValueArray); // 모든 필드값 설정 (특정 필드값 설정은 UTableField에 구현)
	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual void AddField(UTableField* InField);





	// Interface
public:
	UPROPERTY(BlueprintReadWrite)
	bool bIsButtonActivated;

	UFUNCTION()
	virtual void SetActivated(UTableRow* InWidget, bool bIsActivated);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetActivated(bool bIsActivated);

	UFUNCTION()
	void Reset(bool bIsActivated);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecReset();


	// selected
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UTableField*> FocusedFields;
	UFUNCTION()
	void HandleFieldFocus(UTableField* InWidget, bool bIsActivated);
};
