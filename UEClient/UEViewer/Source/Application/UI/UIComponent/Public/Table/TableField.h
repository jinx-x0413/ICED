// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TableManager.h"

#include "TableField.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnFieldFocused, UTableField*, InWidget, bool, bIsActivated);

struct FTableFieldData;


UCLASS()
class UICOMPONENT_API UTableField : public UUserWidget
{
	GENERATED_BODY()

	// construct
public:
	virtual ~UTableField() {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTableFieldData TargetFieldData;

	UFUNCTION()
	void InitializeTableField(FTableFieldData InData);
	UFUNCTION(BlueprintImplementableEvent)
	void ExecInitializeTableField();


	// Feature
public:
	UFUNCTION()
	void InitializeValue(UTableField* InField);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTableRow* ParentTableItem;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnFieldFocused OnFieldFocused;
	UFUNCTION(BlueprintCallable)
	virtual FTableFieldData GetFieldData();
	UFUNCTION(BlueprintImplementableEvent)
	void ExecGetFieldData();
	UFUNCTION(BlueprintCallable)
	virtual void SetFieldValue(const FString& Value);



	// all fields activated
public:
	UFUNCTION()
	void SetFieldChecked(int32 InFieldIndex, bool bIsChecked);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetFieldChecked(bool bIsChecked);

	UFUNCTION()
	void SetFieldSelected(int32 InFieldIndex, int32 InSelectedIndex);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecSetFieldSelected(int32 InSelectedIndex);
};
