// Fill out your copyright notice in the Description page of Project Settings.


#include "Utility/JSONParserForUI.h"
#include "../UIComponentDependency.h"

FString GetFilePath(const FString& InFileName) {
	FString OutFilePath;
	if (GIsEditor)
	{
		OutFilePath = UKismetSystemLibrary::GetProjectDirectory() + "Settings/" + InFileName + ".json";
	}
	else
	{
		OutFilePath = FPaths::LaunchDir() + "Settings/" + InFileName + ".json";
	}

	return OutFilePath;
}

// WBP 클래스를 C++ 코드에서 구조체 변수로 추가하여 저장
UClass* ConvertStringToWBPClass(const FString& InClassString)
{
	
	FString AssetPath = FString::Printf(TEXT("/Game/%s/Table/"), *UIComponentVars::TableAssetPath);
	if (InClassString == "WBP_TableFieldText")
	{
	
		AssetPath = FString::Printf(TEXT("%sWBP_TableFieldText.WBP_TableFieldText_C"), *AssetPath);
		UClass* WBPClass_TableField_Text = LoadClass<UUserWidget>(nullptr, *AssetPath);
		if (WBPClass_TableField_Text)
		{
			return WBPClass_TableField_Text;
		}
	}
	else if (InClassString == "WBP_TableFieldNumber")
	{
		AssetPath = FString::Printf(TEXT("%sWBP_TableFieldNumber.WBP_TableFieldNumber_C"), *AssetPath);
		UClass* WBPClass_TableField_Number = LoadClass<UUserWidget>(nullptr, *AssetPath);
		if (WBPClass_TableField_Number)
		{
			return WBPClass_TableField_Number;
		}
	}
	else if (InClassString == "WBP_TableFieldCheckbox")
	{
		AssetPath = FString::Printf(TEXT("%sWBP_TableFieldCheckbox.WBP_TableFieldCheckbox_C"), *AssetPath);
		UClass* WBPClass_TableField_Checkbox = LoadClass<UUserWidget>(nullptr, *AssetPath);
		if (WBPClass_TableField_Checkbox)
		{
			return WBPClass_TableField_Checkbox;
		}
	}
	else if (InClassString == "WBP_TableFieldCombobox")
	{
		AssetPath = FString::Printf(TEXT("%sWBP_TableFieldCombobox.WBP_TableFieldCombobox_C"), *AssetPath);
		UClass* WBPClass_TableField_Combobox = LoadClass<UUserWidget>(nullptr, *AssetPath);
		if (WBPClass_TableField_Combobox)
		{
			return WBPClass_TableField_Combobox;
		}
	}

	// 기본 클래스 반환 (위젯이 없으면 기본 위젯 반환)
	UE_LOG(LogTemp, Warning, TEXT("Convert Filed Data Class String To WBP Class Failed - Invalid Asset or Asset Path"));
	return UTableFieldText::StaticClass();

}

UClass* ConvertStringToInteractionClass(const FString& InClassString)
{
	if (InClassString == "UCameraInteraction")
	{
		return UCameraInteraction::StaticClass();
	}
	else if (InClassString == "UHiddenInteraction")
	{
		return UHiddenInteraction::StaticClass();
	}
	else if (InClassString == "UHighlightInteraction")
	{
		return UHighlightInteraction::StaticClass();
	}
	else if (InClassString == "UPopupInteraction")
	{
		return UPopupInteraction::StaticClass();
	}
	else if (InClassString == "UTransformInteraction")
	{
		return UTransformInteraction::StaticClass();
	}
	else // default
	{
		return UCameraInteraction::StaticClass();
	}
}


void UJSONParserForUI::ParseJsonComponentTable(const FString& InFileName, FString InTableName, const TArray<FActorHierarchyData>& InActorHierarchyData, FTableData& OutTableData)
{
	FTableData TempTableData = FTableData();
	//TArray<FString> TempFieldArray;

	// 파싱 준비
	FString FilePath = GetFilePath(InFileName);
	FString Content;
	if (!FFileHelper::LoadFileToString(Content, *FilePath))
	{
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to Deserialize Json Data at JsonParser"));
		return;
	}


	// 1. Parse Table
	TempTableData.TableName = InTableName;
	if (JsonObject->HasField(TEXT("Content")))
	{
		TArray<TSharedPtr<FJsonValue>> ContentData = JsonObject->GetArrayField(TEXT("Content"));

		if (InTableName == TEXT("부품설명"))
		{
			TSharedPtr<FJsonObject> TargetContentData = ContentData[0]->AsObject();

			if (TargetContentData->HasField(TEXT("Rows")))
			{
				// 2. parse Row
				TArray<TSharedPtr<FJsonValue>> RowData = TargetContentData->GetArrayField(TEXT("Rows"));
				TSharedPtr<FJsonObject> SampleRowData = RowData[0]->AsObject();
				for (int i = 0; i < InActorHierarchyData.Num(); i++)
				{
					FTableRowData TempRowData;
					if (!InActorHierarchyData[i].TargetComponent.IsValid())
					{
						continue;
					}
					TempRowData.RowIndex = i;
					TempRowData.RowName = InActorHierarchyData[i].NodeName;

					// 3. parse Field
					if (SampleRowData->HasField(TEXT("Fields")))
					{
						TArray<TSharedPtr<FJsonValue>> FieldData = SampleRowData->GetArrayField(TEXT("Fields"));

						for (int j = 0; j < FieldData.Num(); j++)
						{
							FTableFieldData TempFieldData;
							TSharedPtr<FJsonObject> EachFieldData = FieldData[j]->AsObject();
							TempFieldData.FieldIndex = j;

							// field class
							FString TempClassString;
							FString TempFieldName;
							EachFieldData->TryGetStringField(TEXT("FieldClass"), TempClassString);
							TempFieldData.FieldClass = ConvertStringToWBPClass(TempClassString);
							EachFieldData->TryGetStringField(TEXT("FieldName"), TempFieldData.FieldName);
							EachFieldData->TryGetStringField(TEXT("FieldValue"), TempFieldData.FieldValue);


							TempRowData.Fields.Add(TempFieldData);
						}
					}

					TempTableData.Rows.Add(TempRowData);

				}
			}

		}
		else if (InTableName == TEXT("분해/조립"))
		{
			TSharedPtr<FJsonObject> TargetContentData = ContentData[1]->AsObject();

			if (!TargetContentData.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("TargetContentData is not valid at JSONParserForUI"));
			}

			if (TargetContentData->HasField(TEXT("Rows")))
			{
				// 2. parse Row
				TArray<TSharedPtr<FJsonValue>> RowData = TargetContentData->GetArrayField(TEXT("Rows"));
				TSharedPtr<FJsonObject> SampleRowData = RowData[0]->AsObject();
				for (int i = 0; i < InActorHierarchyData.Num(); i++)
				{
					FTableRowData TempRowData;
					if (!InActorHierarchyData[i].TargetComponent.IsValid())
					{
						continue;
					}
					TempRowData.RowIndex = i;
					TempRowData.RowName = InActorHierarchyData[i].NodeName;

					// 3. parse Field
					if (SampleRowData->HasField(TEXT("Fields")))
					{
						TArray<TSharedPtr<FJsonValue>> FieldData = SampleRowData->GetArrayField(TEXT("Fields"));

						for (int j = 0; j < FieldData.Num(); j++)
						{
							FTableFieldData TempFieldData;
							TSharedPtr<FJsonObject> EachFieldData = FieldData[j]->AsObject();
							TempFieldData.FieldIndex = j;

							// field class
							FString TempClassString;
							FString TempFieldName;
							FString TempInteractionClassString;
							EachFieldData->TryGetStringField(TEXT("FieldClass"), TempClassString);
							TempFieldData.FieldClass = ConvertStringToWBPClass(TempClassString);
							EachFieldData->TryGetStringField(TEXT("FieldName"), TempFieldData.FieldName);
							EachFieldData->TryGetStringField(TEXT("FieldValue"), TempFieldData.FieldValue);
							EachFieldData->TryGetStringField(TEXT("InteractionClassName"), TempInteractionClassString);
							TempFieldData.InteractionClass = ConvertStringToInteractionClass(TempInteractionClassString);
							
							//if (TempFieldData.FieldIndex == 0) // Step Index
							//{
							//	TempFieldData.FieldValue = FString::FromInt(i);
							//}

							TempRowData.Fields.Add(TempFieldData);
						}
					}

					TempTableData.Rows.Add(TempRowData);

				}
			}
		}
	}




	OutTableData = TempTableData;
	return;
}

