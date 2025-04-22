// Fill out your copyright notice in the Description page of Project Settings.


#include "Template/Assembly/AssemblyBuilder.h"
#include "../TemplateBuildDependency.h"

UAssemblyBuilder::UAssemblyBuilder()
	: ClipInterval(2.0f)
	, TrackInterval(4 * ClipInterval)
{
}

UAssemblyBuilder::~UAssemblyBuilder()
{
}

void UAssemblyBuilder::Build()
{
	
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
		TargetTableData = UTableManager::GetTableManager()->GetTableData(); // 참조로 사용

		for (int i = 0; i < CurrentData.Num() - 1; i++) // Actor 대상으로 클립 생성하지 않도록
		{
			for (auto& Row : TargetTableData.Rows)
			{
				if (Row.RowName == CurrentData[i + 1].DisplayName)
				{
					
					if (CurrentData[i + 1].TargetComponent.IsValid())
					{
						//UTrack* NewTrack = UTimebarPlayer::GetTimebarPlayer()->CreateTrack(Controller->TrackHeaderWidgetClass, Controller->TrackWidgetClass, CurrentData[i + 1].DisplayName);
						UTrack* NewTrack = nullptr;
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

									if(Field.FieldValue == TEXT("false"))
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
					}


					break;
				}
			}






			//SetTracks(CurrentData[i + 1], (float) i * TrackInterval);
		}
		
		
	}

	
}


float UAssemblyBuilder::SetClip(
	UTrack* InTrack
	, TSubclassOf<UInteractionBase> InInteractionClass
	, FName InInteractionName
	, float PrevTrackEndTime
	, FString InFieldValue)
{
	float CurrentEndTrackTime = PrevTrackEndTime;

	if (IsValid(Controller))
	{
		USceneComponent* TargetComponent = InTrack->TargetComponent;

		FInteractionData TargetInteractionData;
		TargetInteractionData.TargetActor = TargetActor;
		TargetInteractionData.TargetComponent = TargetComponent;
		TargetInteractionData.Name = InInteractionName;
		TargetInteractionData.TargetClass = InInteractionClass;


		TargetInteractionData.bIsHighlighted = true;
		/*TargetInteractionData.StartTransform = FTransform(FRotator(), TargetComponent->GetComponentLocation(), FVector());
		TargetInteractionData.EndTransform = FTransform(FRotator(), TargetComponent->GetComponentLocation() + FVector(0, 0, 250.0f), FVector());*/

		// transform interaction set direction
		ETransformInteractionDirection CurrentDirection = ETransformInteractionDirection::AUTO;
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
		TargetInteractionData.TransformDirection = CurrentDirection;

		TargetInteractionData.bIsHidden = true;
		// TODO : refactoring ; switch?
		// TODO : popup Interaction property (TargetWidget, Text)
		

		UInteractionBase* NewCameraInteraction = Controller->CreateInteraction(
			TargetInteractionData
			, InTrack
			, PrevTrackEndTime
			, PrevTrackEndTime + ClipInterval);

		CurrentEndTrackTime = PrevTrackEndTime + ClipInterval;
	}

	return CurrentEndTrackTime;
}