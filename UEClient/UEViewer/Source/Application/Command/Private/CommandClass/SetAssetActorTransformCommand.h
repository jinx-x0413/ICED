// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommandClass/CommandBase.h"
#include "SetAssetActorTransformCommand.generated.h"

/**
 * 
 */
UCLASS()
class USetAssetActorTransformCommand : public UCommandBase
{
	GENERATED_BODY()
	// construct




// feature
public:
	virtual void Execute() override;

	virtual void Undo() override;
};
