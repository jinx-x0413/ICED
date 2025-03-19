// Fill out your copyright notice in the Description page of Project Settings.


#include "ProfilingPlayerController.h"
#include "../Private/Profiler.h"
#include "Kismet/GameplayStatics.h"


AProfilingPlayerController::AProfilingPlayerController()
    : Profiler(CreateDefaultSubobject<UProfiler>(TEXT("Profiler")))
{

}

void AProfilingPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (GetWorld()->IsPaused() && !bIsInputPressed)
    {
        CheckKeyboardInput();
    }
    else if (WasInputKeyJustReleased(EKeys::AnyKey)) // SetGamePaused(true) �ϰ� ���� ���ε� �� Ű ��ü�� �Էµ��� ����. Tick���� �����Ͽ� �����ϴ� ���� �߰�
    {
        ReleaseKeyboardInput();
    }
}

void AProfilingPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // Bind the input action
    //InputComponent->BindAction("ProfilingStart", IE_Pressed, this, &AProfilingPlayerController::HandleInputAction);
    InputComponent->BindAction("ProfilingStart", IE_Pressed, this, &AProfilingPlayerController::HandleProfilingStart);
    InputComponent->BindAction("ProfilingSwitch", IE_Pressed, this, &AProfilingPlayerController::HandleProfilingSwitch);
    InputComponent->BindAction("ProfilingSlow", IE_Pressed, this, &AProfilingPlayerController::HandleProfilingSlow);
    InputComponent->BindAction("ProfilingShowFlag", IE_Pressed, this, &AProfilingPlayerController::HandleProfilingShowFlag);
    InputComponent->BindAction("ProfilingShowFlagWidget", IE_Pressed, this, &AProfilingPlayerController::HandleProfilingShowFlagWidget);
    InputComponent->BindAction("ProfilingPause", IE_Pressed, this, &AProfilingPlayerController::HandleProfilingPause);
    InputComponent->BindAction("ProfilingRecord", IE_Pressed, this, &AProfilingPlayerController::HandleProfilingRecord);
}

bool AProfilingPlayerController::CheckKeyboardInput()
{
    if (IsInputKeyDown(EKeys::LeftShift) && IsInputKeyDown(EKeys::One))
    {
        HandleProfilingStart();
    }
    else if (IsInputKeyDown(EKeys::LeftShift) && IsInputKeyDown(EKeys::Two))
    {
        HandleProfilingSwitch();
    }
    else if (IsInputKeyDown(EKeys::LeftShift) && IsInputKeyDown(EKeys::Three))
    {
        HandleProfilingSlow();
    }
    else if (IsInputKeyDown(EKeys::LeftShift) && IsInputKeyDown(EKeys::H))
    {
        HandleProfilingShowFlag();
    }
    else if (IsInputKeyDown(EKeys::LeftShift) && IsInputKeyDown(EKeys::E))
    {
        HandleProfilingShowFlagWidget();
    }
    else if (IsInputKeyDown(EKeys::LeftShift) && IsInputKeyDown(EKeys::SpaceBar))
    {
        HandleProfilingPause();
    }
    else if (WasInputKeyJustReleased(EKeys::AnyKey)) // ���� ���� : Ű �Է��� Ǯ���� �ʾ��� ���
    {
        ReleaseKeyboardInput();
    }

    return true;
}

void AProfilingPlayerController::ReleaseKeyboardInput()
{
    bIsInputPressed = false;
    //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Released Keyboard Input")));
}

void AProfilingPlayerController::HandleProfilingStart()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingStart;
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Shift + 1 pressed")));
        if (Profiler)
        {
            Profiler->SetProfilingStat(this);
        }
    }

}

void AProfilingPlayerController::HandleProfilingSwitch()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingSwitch;
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Shift + 2 pressed")));
        if (Profiler)
        {
            Profiler->SetProfilingStat(this);
        }
    }
}

void AProfilingPlayerController::HandleProfilingSlow()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingSlow;
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Shift + 3 pressed")));
        if (Profiler)
        {
            Profiler->SetProfilingStat(this);
        }
    }
}

void AProfilingPlayerController::HandleProfilingShowFlag()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingShowFlag;
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Shift + H pressed")));
        if (Profiler)
        {
            Profiler->SetProfilingStat(this);
        }
    }
}

void AProfilingPlayerController::HandleProfilingShowFlagWidget()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingShowFlagWidget;
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Shift + W pressed")));
        if (Profiler)
        {
            Profiler->SetProfilingStat(this);
        }
    }
}

// FIXME : Pause ���¿����� ��ǲ�� ���� �� �ֵ��� ����
void AProfilingPlayerController::HandleProfilingPause()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingPause;
        bIsPaused = !bIsPaused;
        UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);

        // TODO : �ڵ� ���� �� ����� ��� ����
        if (bIsPaused)
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Paused !")));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Released !")));
        }
    }


}

void AProfilingPlayerController::HandleProfilingRecord()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingRecord;
        //GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Shift + R pressed")));
        if (Profiler)
        {
            Profiler->SetProfilingStat(this);
        }
    }
}
