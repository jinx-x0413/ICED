// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Clip.generated.h"

/**
 * 
 */
class UClipWidget;

UCLASS()
class TIMEBAR_API UClip : public UObject
{
	GENERATED_BODY()

    // construct
public:
    UClip();
    virtual ~UClip();




	
public:
    // 클립의 시작 시간
    float StartTime;

    // 클립의 끝 시간
    float EndTime;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float ClipLength;

    bool bIsPlayable = true; // 재생 가능한 클립인지 여부

    // 클립이 현재 재생 중인지 여부
    bool bIsPlaying = false;

    // 클립의 위치 (타임라인 상)
    FVector2D Position;

    // 현재 시간이 클립의 재생 시간에 해당하는지 체크하는 함수
    bool ShouldPlay(float CurrentTime);

    bool SetPlayable(bool InbIsPlayable);


    UFUNCTION()
    virtual void Play(float InCurrentTime);

    UFUNCTION()
    virtual void Pause();

    UFUNCTION()
    virtual void Stop();


    // widget
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UClipWidget* TargetWidget;

};
