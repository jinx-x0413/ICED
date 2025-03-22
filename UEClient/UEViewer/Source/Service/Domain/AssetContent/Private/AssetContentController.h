// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AssetContentController.generated.h"

class UInteractionBase;


UCLASS()
class UAssetContentController : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UAssetContentController();
	virtual ~UAssetContentController();



	// business logic
public:
	virtual UInteractionBase* CreateInteraction(TSubclassOf<UInteractionBase> InInteractionClass);

};
