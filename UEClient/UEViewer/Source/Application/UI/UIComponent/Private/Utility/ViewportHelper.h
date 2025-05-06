// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ViewportHelper.generated.h"

/**
 * 
 */
UCLASS()
class UViewportHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Viewport Helper")
	static FVector2D AdjustedPosition(FVector2D InPosition);
};
