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
    // Ŭ���� ���� �ð�
    float StartTime;

    // Ŭ���� �� �ð�
    float EndTime;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float ClipLength;

    bool bIsPlayable = true; // ��� ������ Ŭ������ ����

    // Ŭ���� ���� ��� ������ ����
    bool bIsPlaying = false;

    // Ŭ���� ��ġ (Ÿ�Ӷ��� ��)
    FVector2D Position;

    // ���� �ð��� Ŭ���� ��� �ð��� �ش��ϴ��� üũ�ϴ� �Լ�
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
