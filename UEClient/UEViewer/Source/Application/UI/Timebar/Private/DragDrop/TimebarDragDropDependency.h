// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "TimebarDragDropOperation.h"
#include "TimebarDraggableWidget.h"
#include "TimebarDroppableWidget.h"

#include "Track/TrackWidget.h"
#include "Track/ClipWidget.h"
#include "Track/SideTrackWidget.h"

#include "TimeScale/TimeScaleWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

#include "TimebarDragDropDependency.generated.h"

namespace Variables
{
	extern float SnapInterval;
	extern float ConvertedValue;
	extern float ScaleMultiplier;
	extern float MinClipLength;
	extern float ResizeThreshold;
	extern float InitialRulerSpacing;
	extern float InitialZoomScale;
	extern float MaxZoomScale;
	extern float MinZoomScale;
	extern float CursorInterval;
}

UCLASS()
class UTimebarDragDropDependency : public UObject
{
	GENERATED_BODY()
	
};
