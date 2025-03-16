// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CommandBase.generated.h"

/**
 * 
 */
UCLASS()
class UCommandBase : public UObject
{
	GENERATED_BODY()
	
	// construct




	// feature
public:
	UFUNCTION()
	virtual void Execute();

	UFUNCTION()
	virtual void Undo();
};
