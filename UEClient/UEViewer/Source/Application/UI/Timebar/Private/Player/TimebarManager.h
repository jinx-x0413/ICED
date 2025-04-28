// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimebarManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UTimebarManager : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UTimebarManager();
	virtual ~UTimebarManager();
	virtual void BeginDestroy() override;
	void Shutdown();


	// feature
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UTrack*> TrackArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray <class UClip* > ClipArray;
};
