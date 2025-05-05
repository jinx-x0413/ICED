// Fill out your copyright notice in the Description page of Project Settings.


#include "Template/Assembly/DisassemblyBuilder.h"
#include "../TemplateBuildDependency.h"


UDisassemblyBuilder::UDisassemblyBuilder()
	: ClipInterval(1.0f)
{
}

UDisassemblyBuilder::~UDisassemblyBuilder()
{
}

void UDisassemblyBuilder::BeginDestroy()
{
	Super::BeginDestroy();
}




// interface
void UDisassemblyBuilder::Build()
{// Get AssetActor Hierarchy Data
	if (IsValid(TargetActor))
	{
		TArray<FActorHierarchyData> CurrentData;
		CurrentData.Empty(0);

		for (auto Data : TargetActor->HierarchyData)
		{
			CurrentData.Add(Data);
		}

		float LatestClipEndTime = 0.0f;
		//TargetTableData = UTableController::GetTableManager(TEXT("Disassembly"))->GetTableData(); // 참조로 사용

		for (int i = 0; i < CurrentData.Num() - 1; i++) // Actor 대상으로 클립 생성하지 않도록
		{
			for (auto& Row : TargetTableData.Rows)
			{
				if (Row.RowName == CurrentData[i + 1].DisplayName)
				{

					if (CurrentData[i + 1].TargetComponent.IsValid())
					{
						//UTrack* NewTrack = UTimebarPlayer::GetTimebarPlayer()->CreateTrack(Controller->TrackHeaderWidgetClass, Controller->TrackWidgetClass, CurrentData[i + 1].DisplayName);
						UTrack* NewTrack = UTimebarPlayer::GetTimebarPlayer()->CreateComponentListItem(
							Controller->TrackSceneCaptureWidgetClass
							, CurrentData[i + 1].DisplayName);
						if (IsValid(NewTrack))
						{
							if (IsValid(CurrentData[i + 1].TargetComponent.Get()))
							{
								NewTrack->TargetComponent = CurrentData[i + 1].TargetComponent.Get();

								// loop table data
								for (auto& Field : Row.Fields)
								{
									// check if data checked
									//UE_LOG(LogTemp, Warning, TEXT("AssemblyBuilder Field Value: %s - %s:%s"), *Row.RowName, *Field.FieldName,  *Field.FieldValue);
									if (Field.FieldValue.IsEmpty())
									{
										continue;
									}


									if (Field.FieldValue == TEXT("false"))
									{
										UE_LOG(LogTemp, Warning, TEXT("AssemblyBuilder Field Value: %s - %s:%s"), *Row.RowName, *Field.FieldName, *Field.FieldValue);
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
							}


						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("New Track is Invalid at AssemblyBuilder"));
						}

						UTimebarPlayer::GetTimebarPlayer()->OnComponentListItemCreated.Broadcast(NewTrack, NewTrack->SceneCaptureWidget);
					}

					break;
				}
			}






			//SetTracks(CurrentData[i + 1], (float) i * TrackInterval);
		}


	}
}



// feature


float UDisassemblyBuilder::SetClip(UTrack* InTrack, TSubclassOf<UInteractionBase> InInteractionClass, FName InInteractionName, float PrevTrackEndTime, FString InFieldValue, TSubclassOf<UUserWidget> InPopupWidgetClass)
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

		// hidden interaction
		TargetInteractionData.bIsHidden = true;

		// create
		UInteractionBase* NewInteraction = Controller->CreateInteraction(
			InInteractionClass
			, TargetInteractionData
			, InTrack
			, PrevTrackEndTime
			, PrevTrackEndTime + ClipInterval);

		CurrentEndTrackTime = PrevTrackEndTime + ClipInterval;

		// popupWidget interaction
		if (InInteractionClass == UPopupInteraction::StaticClass())
		{
			UTemplateHandler::GetTemplateHandler()->OnPopupInteractionCreated.Broadcast(NewInteraction, InPopupWidgetClass, InFieldValue);
		}

	}

	return CurrentEndTrackTime;
}
