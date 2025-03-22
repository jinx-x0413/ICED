// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Track.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UTrack : public UObject
{
	GENERATED_BODY()


	// construct
public:
	UTrack();
	virtual ~UTrack();





	// component
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* TargetComponent;



	// SideTracks
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTrack* ParentTrack;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UTrack*> SideTracks;

	
	// Clips
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UClip*> Clips;

	UFUNCTION()
	void AddClipToArray(UTrack* InTrack, UClip* InClip);

	UFUNCTION()
	void RemoveClip(UClip* InClip);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPlayable = true;

	UFUNCTION()
	void SetClipsPlayable(bool InbIsPlayable);

	UFUNCTION(BlueprintCallable)
	void ToggleClipsPlayable();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector2D GetMinMaxTime();

	


	// Timebar Widgets
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* HeaderWidget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UTrackWidget* ContentWidget;


};
