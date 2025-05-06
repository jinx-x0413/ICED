// Fill out your copyright notice in the Description page of Project Settings.


#include "Template/Assembly/AssemblyBuilder.h"
#include "../TemplateBuildDependency.h"

// construct
UAssemblyBuilder::UAssemblyBuilder()
	: ClipInterval(1.0f)

{
}

UAssemblyBuilder::~UAssemblyBuilder()
{
}

void UAssemblyBuilder::BeginDestroy()
{
	Super::BeginDestroy();
}





void UAssemblyBuilder::Build()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Build Assembly Content"));

	// Get AssetActor Hierarchy Data
	if (IsValid(TargetActor))
	{
		TArray<FActorHierarchyData> CurrentData;
		CurrentData.Empty(0);

		for (auto Data : TargetActor->HierarchyData)
		{
			CurrentData.Add(Data);
		}

		float LatestClipEndTime = 0.0f;
		//TargetTableData = UTableController::GetTableManager(TEXT("Assembly"))->GetTableData(); // 참조로 사용

		for (int i = 0; i < TargetTableData.Rows.Num(); i++) // Actor 대상으로 클립 생성하지 않도록
		{
			FTableRowData TargetRowData = TargetTableData.Rows[TargetTableData.Rows.Num() - i - 1];
			for (int j = 0; j < CurrentData.Num() - 1; j++)
			{
				//if(TargetRowData.RowName == CurrentData[j + 1].NodeName)
				if (TargetRowData.RowName == CurrentData[j + 1].DisplayName)
				{
					//UTrack* NewTrack = UTimebarPlayer::GetTimebarPlayer()->CreateTrack(Controller->TrackHeaderWidgetClass, Controller->TrackWidgetClass, TargetRowData.RowName);
					UTrack* NewTrack = UTimebarPlayer::GetTimebarPlayer()->CreateComponentListItem(
						Controller->TrackSceneCaptureWidgetClass
						, CurrentData[i + 1].DisplayName);
					if (IsValid(NewTrack))
					{
						if (IsValid(CurrentData[j + 1].TargetComponent.Get()))
						{
							NewTrack->TargetComponent = CurrentData[j + 1].TargetComponent.Get();
							// loop table data
							for (auto& Field : TargetRowData.Fields)
							{
								// check if data checked
								if (Field.FieldValue.IsEmpty())
								{
									continue;
								}


								if (Field.FieldValue == TEXT("false"))
								{
									UE_LOG(LogTemp, Warning, TEXT("AssemblyBuilder Field Value: %s - %s:%s"), *TargetRowData.RowName, *Field.FieldName, *Field.FieldValue);
								}
								else if (Field.FieldValue == TEXT("true"))
								{
									LatestClipEndTime = SetClip(NewTrack, Field.InteractionClass, FName(*Field.FieldName), LatestClipEndTime);
								}
								else if (Field.InteractionClass == UTransformInteraction::StaticClass() && Field.FieldValue != TEXT("None")) // transform interaction
								{
									LatestClipEndTime = SetClip(NewTrack, Field.InteractionClass, FName(*Field.FieldName), LatestClipEndTime, Field.FieldValue);
								}
								else if (Field.InteractionClass == UPopupInteraction::StaticClass() && !Field.FieldValue.IsEmpty()) // popup interaction
								{
									LatestClipEndTime = SetClip(NewTrack, Field.InteractionClass, FName(*Field.FieldName), LatestClipEndTime, Field.FieldValue, Field.PopupWidgetClass);
									// create popupwidget
								}
							}
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("InHierarchyData.TargetComponent is Invalid at AssemblyBuilder"));
#if UE_BUILD_SHIPPING
							FString LogString = FString::Printf(TEXT("InHierarchyData.TargetComponent is Invalid at AssemblyBuilder"));

							FString LogFilePath = FPaths::ProjectLogDir() + TEXT("ShippingLog.txt");

							// Append 모드로 여러 줄 저장 가능하게
							FFileHelper::SaveStringToFile(LogString, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
						}
					}
					else
					{
						UE_LOG(LogTemp, Warning, TEXT("New Track is Invalid at AssemblyBuilder"));
#if UE_BUILD_SHIPPING
						FString LogString = FString::Printf(TEXT("New Track is Invalid at AssemblyBuilder"));

						FString LogFilePath = FPaths::ProjectLogDir() + TEXT("ShippingLog.txt");

						// Append 모드로 여러 줄 저장 가능하게
						FFileHelper::SaveStringToFile(LogString, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
					}

					UTimebarPlayer::GetTimebarPlayer()->OnComponentListItemCreated.Broadcast(NewTrack, NewTrack->SceneCaptureWidget);

#if UE_BUILD_SHIPPING
					FString LogString = FString::Printf(TEXT("OnComponentListItemCreated Broadcasted at AssemblyBuilder"));

					FString LogFilePath = FPaths::ProjectLogDir() + TEXT("ShippingLog.txt");

					// Append 모드로 여러 줄 저장 가능하게
					FFileHelper::SaveStringToFile(LogString, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
				}

			}

		}


	}
	else
	{
#if UE_BUILD_SHIPPING
		FString LogString = FString::Printf(TEXT("TargetActor is Invalid at AssemblyBuilder"));

		FString LogFilePath = FPaths::ProjectLogDir() + TEXT("ShippingLog.txt");

		// Append 모드로 여러 줄 저장 가능하게
		FFileHelper::SaveStringToFile(LogString, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
	}


}


float UAssemblyBuilder::SetClip(
	UTrack* InTrack
	, TSubclassOf<UInteractionBase> InInteractionClass
	, FName InInteractionName
	, float PrevTrackEndTime
	, FString InFieldValue
	, TSubclassOf<UUserWidget> InPopupWidgetClass)
{
	float CurrentEndTrackTime = PrevTrackEndTime;

	if (IsValid(Controller))
	{
		USceneComponent* TargetComponent = InTrack->TargetComponent;

		// common
		FInteractionData TargetInteractionData;
		TargetInteractionData.TargetActor = TargetActor;
		TargetInteractionData.TargetComponent = TargetComponent;
		TargetInteractionData.Name = InInteractionName;

		// highlight interaction
		TargetInteractionData.bIsHighlighted = true;


		// transform interaction set direction
		ETransformInteractionDirection CurrentDirection = ETransformInteractionDirection::AUTO;
		if (InInteractionClass == UTransformInteraction::StaticClass())
		{
			if (InFieldValue == TEXT("Up"))
			{
				CurrentDirection = ETransformInteractionDirection::UP;
			}
			else if (InFieldValue == TEXT("Down"))
			{
				CurrentDirection = ETransformInteractionDirection::DOWN;
			}
			else if (InFieldValue == TEXT("Left"))
			{
				CurrentDirection = ETransformInteractionDirection::LEFT;
			}
			else if (InFieldValue == TEXT("Right"))
			{
				CurrentDirection = ETransformInteractionDirection::RIGHT;
			}
		}
		TargetInteractionData.TransformDirection = CurrentDirection;
		TargetInteractionData.bIsReversed = true;

		// hidden interaction
		TargetInteractionData.bIsHidden = false;

		// create
		float CurrentInterval = ClipInterval;
		if (InInteractionClass == UTransformInteraction::StaticClass())
		{
			CurrentInterval = 2.0f;
		}
		UInteractionBase* NewInteraction = Controller->CreateInteraction(
			InInteractionClass
			, TargetInteractionData
			, InTrack
			, PrevTrackEndTime
			, PrevTrackEndTime + CurrentInterval);

		CurrentEndTrackTime = PrevTrackEndTime + CurrentInterval;

		// popupwidget interaction
		if (InInteractionClass == UPopupInteraction::StaticClass())
		{
			UTemplateHandler::GetTemplateHandler()->OnPopupInteractionCreated.Broadcast(NewInteraction, InPopupWidgetClass, InFieldValue);
		}

	}

	return CurrentEndTrackTime;
}
