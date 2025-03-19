// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../../Public/DragDrop/DraggableWidget.h"
#include "../../Public/DragDrop/DroppableWidget.h"
#include "../../Public/DragDrop/MyDragDropOperation.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"

#include "DragDropDependency.generated.h"

/**
 * 
 */

namespace DragDropVars {
	extern float SnapInterval;
}

UCLASS()
class UDragDropDependency : public UObject
{
	GENERATED_BODY()
	
};
