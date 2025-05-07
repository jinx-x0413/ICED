// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JSONParserForUI.generated.h"

struct FDropdownData;
struct FActorHierarchyData;
 
UCLASS()
class UICOMPONENT_API UJSONParserForUI : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "JsonParser")
	static void ParseJsonComponentTable(const FString& InFileName, FString InTableName, const TArray<FActorHierarchyData>& InActorHierarchyData, FTableData& OutTableData);

};
