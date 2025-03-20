// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommandHistory.generated.h"

/**
 * 
 */
UCLASS()
class UCommandHistory : public UObject
{
	GENERATED_BODY()
	
	// construct




	// feature
private:
	UPROPERTY()
	TArray<UCommandBase*> Commands;

public:
	UFUNCTION(meta=(AllowPrivateAccess=true))
	TArray<UCommandBase*> GetCommands() const { return Commands; }

	UFUNCTION(meta = (AllowPrivateAccess = true))
	void AddCommandToHistory(UCommandBase* InCommand);

	UFUNCTION(meta = (AllowPrivateAccess = true))
	UCommandBase* PopCommand();
};
