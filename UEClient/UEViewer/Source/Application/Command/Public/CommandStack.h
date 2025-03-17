// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommandStack.generated.h"

/**
 * 
 */
UCLASS()
class COMMAND_API UCommandStack : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UCommandStack();
	
	virtual ~UCommandStack();



	// feature
private:
	UPROPERTY()
	UCommandHistory* History;

public:
	UFUNCTION(meta = (AllowPrivateAccess=true))
	UCommandHistory* GetHistory();

	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual void AddCommandToHistory(UCommandBase* InCommand);

	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual void ExecuteCommand(UCommandBase* InCommand);

	UFUNCTION(meta = (AllowPrivateAccess = true))
	virtual void Undo();
};
