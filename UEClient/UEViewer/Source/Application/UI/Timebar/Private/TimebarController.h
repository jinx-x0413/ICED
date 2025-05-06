// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "TimebarController.generated.h"

USTRUCT()
struct FTimebarManagerWrapper
{
	GENERATED_BODY()

	UPROPERTY()
	UTimebarManager* Manager;

	FTimebarManagerWrapper()
		: Manager(nullptr)
	{}
};

UCLASS(BlueprintType)
class TIMEBAR_API UTimebarController : public UObject
{
	GENERATED_BODY()
	
	// construct
public:
	UTimebarController();
	virtual ~UTimebarController();
	virtual void BeginDestroy() override;
	void Shutdown();


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

	//// Timebar Manager
public:
	UFUNCTION(BlueprintCallable)
	UTimebarManager* GetTimebarManager(FName InManagerName);

	TMap<FName, FTimebarManagerWrapper> ManagerMap;

	UFUNCTION(BlueprintCallable)
	void SetTimebarManager(FName InManagerName);
};
