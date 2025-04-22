// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "DragDrop/DragDropDependency.h"
#include "Table/TableDependency.h"
#include "SceneCapture/SceneCaptureDependency.h"

#include "Kismet/KismetSystemLibrary.h"

#include "Service/Domain/AssetActor/Public/AssetActor.h"
#include "Service/Domain/AssetContent/Public/AssetContent.h"

// json header
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonWriter.h"

#include "UIComponentDependency.generated.h"


namespace UIComponentVars
{
	extern FString TableAssetPath;
}

UCLASS()
class UUIComponentDependency : public UObject
{
	GENERATED_BODY()
	
};
