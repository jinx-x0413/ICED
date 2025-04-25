// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TimebarPlayer.generated.h"

UENUM(BlueprintType)
enum class ETimebarState : uint8
{
	Stopped		 UMETA(DisplayName = "Stopped"),
	Paused		 UMETA(DisplayName = "Paused"),
	Running		 UMETA(DisplayName = "Running")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTrackCreated, UTrack*, InTrack, UUserWidget*, InHeaderWidget, UTrackWidget*, InTrackWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnSideTrackCreated, UTrack*, InParentTrack, UTrack*, InTrack, UUserWidget*, InHeaderWidget, USideTrackWidget*, InTrackWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackDeleted, UTrack*, InTrack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackSelected, UTrack*, InTrack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComponentTrackCreated, UTrack*, InTrack, USceneCaptureIcon*, InSceneCaptureWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnComponentTrackBackwardCreated, UTrack*, InTrack, USceneCaptureIcon*, InSceneCaptureWidget);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClipCreated, UTrack*, InTrack, UClip*, InClip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClipDeleted, UClip*, InClip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClipSelected, UClip*, InClip);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFinished);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentTimeChanged, float, InCurrentTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChanged, ETimebarState, InState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnScaleChanged, float, InOldScale, float, InNewScale);

UCLASS(BlueprintType)
class TIMEBAR_API UTimebarPlayer : public UObject
{
	GENERATED_BODY()

public:
	UTimebarPlayer();
	void BeginDestroy();
	virtual ~UTimebarPlayer();

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = true))
	static UTimebarPlayer* GetTimebarPlayer();

	UFUNCTION()
	void Shutdown();

private:
	static UTimebarPlayer* Instance;

public:
	// Track
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UTrack*> TrackArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UTrack*> ReverseTrackArray;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTrackCreated OnTrackCreated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnSideTrackCreated OnSideTrackCreated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTrackDeleted OnTrackDeleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnTrackSelected OnTrackSelected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UTrack* SelectedTrack;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnComponentTrackCreated OnComponentTrackCreated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnComponentTrackBackwardCreated OnComponentTrackBackwardCreated;

	UFUNCTION()
	UTrack* CreateTrack(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName);

	UFUNCTION()
	UTrack* CreateSideTrack(UTrack* InParentTrack, TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName);

	UFUNCTION()
	UTrack* CreateComponent(TSubclassOf<UUserWidget> InSceneCaptureWidget, FString InName, USkeletalMeshComponent* InTargetComponent);

	UFUNCTION()
	UTrack* CreateComponentBackward(TSubclassOf<UUserWidget> InSceneCaptureWidget, FString InName, USkeletalMeshComponent* InTargetComponent);

	UFUNCTION(BlueprintCallable)
	void SelectTrack(UTrack* InTrack);

	UFUNCTION()
	void DeleteTrack(UTrack* InTrack);

	// Clip
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UClip*> ClipArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<UClip*> ReverseClipArray;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnClipCreated OnClipCreated;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnClipDeleted OnClipDeleted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnClipSelected OnClipSelected;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClip* SelectedClip;

	UFUNCTION(BlueprintCallable)
	UClip* CreateClip(UTrack* InTrack, float InStartTime, float InEndTime, FString InName);

	UFUNCTION()
	void AddClipToArray(UTrack* InTrack, UClip* InClip);

	UFUNCTION(BlueprintCallable)
	void SelectClip(UClip* InClip);

	UFUNCTION(BlueprintCallable)
	void DeleteClip(UClip* InClip);

	// Player
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnCurrentTimeChanged OnCurrentTimeChanged;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnStarted OnStarted;

	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnFinished OnFinished;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnScaleChanged OnScaleChanged;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETimebarState State = ETimebarState::Stopped;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsPlayingBackward = false;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnStateChanged OnStateChanged;

	UPROPERTY(BlueprintReadOnly)
	float CurrentTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsLooping = false;

	UPROPERTY()
	FTimerHandle MainTimer;

	UFUNCTION(BlueprintCallable)
	void Start();

	UFUNCTION(BlueprintCallable)
	void Pause();

	UFUNCTION(BlueprintCallable)
	void Stop();

	UFUNCTION(BlueprintCallable)
	void Run();

	UFUNCTION(BlueprintCallable)
	void SetCurrentTime(float InCurrentTime);

	UFUNCTION(BlueprintCallable)
	void RunBackward();
};