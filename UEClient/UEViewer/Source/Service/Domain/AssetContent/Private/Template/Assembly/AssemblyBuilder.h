// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "../TemplateInterface.h"

#include "AssemblyBuilder.generated.h"


class AGltfAssetActor;
class UTrack;
struct FActorHierarchyData;


UCLASS()
class UAssemblyBuilder : public UObject, public ITemplateInterface
{
	GENERATED_BODY()
	
	// construct
public:
	UAssemblyBuilder();
	virtual ~UAssemblyBuilder();



	// interface
public:
	virtual void Build() override;





	// feature
public:
	float ClipInterval = 2.0f;
	float TrackInterval = 8.0f;
	//void SetTracks(FActorHierarchyData InHierarchyData, float PrevTrackEndTime);

	float SetClip(
		UTrack* InTrack
		, TSubclassOf<UInteractionBase> InInteractionClass
		, FName InInteractionName
		, float PrevTrackEndTime
		, FString InFieldValue = "Auto");

	//void SetClips(UTrack* InTargetTrack, float PrevTrackEndTime); // 영상 촬영용 하드코딩 함수
	
};
