// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "TimebarController.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UTimebarController : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UTimebarController();
	virtual ~UTimebarController();



	// Business Logic
public:
	UFUNCTION(BlueprintCallable)
	virtual void CreateTrack(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName);

	UFUNCTION(BlueprintCallable)
	virtual void DeleteTrack(UUserWidget* InTrackWidget);

	UFUNCTION(BlueprintCallable)
	virtual void CreateClip(UUserWidget* InTrackWidget, float InStartTime, float InEndTime, FString InName);

	UFUNCTION(BlueprintCallable)
	virtual void DeleteClip(UUserWidget* InClipWidget);

	UFUNCTION(BlueprintCallable)
	virtual void Start();

	UFUNCTION(BlueprintCallable)
	virtual void Pause();

	UFUNCTION(BlueprintCallable)
	virtual void Stop();

	UFUNCTION(BlueprintCallable)
	virtual void SetCurrentTime(float InCurrentTime);
};
