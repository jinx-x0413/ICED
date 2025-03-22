// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "TimebarDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class UTimebarDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()

public:
    UPROPERTY()
    UUserWidget* SourceWidget;

    UPROPERTY()
    FVector2D DragOffset;
};
