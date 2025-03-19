// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProfilingPlayerController.generated.h"

class UProfiler;

UENUM()
enum class EInputAction
{
	ProfilingStart,
	ProfilingSwitch,
	ProfilingSlow,
	ProfilingShowFlag,
	ProfilingShowFlagWidget,
	ProfilingPause,
	ProfilingRecord
};


UCLASS()
class PROFILING_API AProfilingPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	AProfilingPlayerController();




	// keyboard input setting
protected:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupInputComponent() override;

private:
	bool bIsPaused = false;
	bool bIsInputPressed = false;
	bool CheckKeyboardInput();
	void ReleaseKeyboardInput();




	// handle profiling
public:

	EInputAction CurrentAction = EInputAction::ProfilingStart;

private:
	void HandleProfilingStart();
	void HandleProfilingSwitch();
	void HandleProfilingSlow();
	void HandleProfilingShowFlag();
	void HandleProfilingShowFlagWidget();
	void HandleProfilingPause();
	void HandleProfilingRecord();

	TObjectPtr<UProfiler> Profiler;
};
