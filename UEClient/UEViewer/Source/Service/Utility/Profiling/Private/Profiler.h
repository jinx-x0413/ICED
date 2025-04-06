// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Profiler.generated.h"

UENUM()
enum class EThread
{
	None,
	GameThread,
	RenderingThread,
	GPU

};

UENUM()
enum class EFlag
{
	None,
	SkeletalMesh,
	StaticMesh,
	Particle
};


UCLASS()
class UProfiler : public UObject
{
	GENERATED_BODY()

protected:
	UProfiler();




	// profiling command
public:
	UFUNCTION(meta = (AllowPrivateAccess = true))
	void SetProfilingStat(AProfilingPlayerController* PC);

private:
	void ExecuteConsoleCommand(APlayerController* PC, const FString Command);
	bool bIsStatToggled = false;
	bool bIsRecording = false;
	EThread CurrentThread = EThread::None;
	EFlag CurrentFlag = EFlag::None;
};
