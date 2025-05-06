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
    else if (WasInputKeyJustReleased(EKeys::AnyKey)) // SetGamePaused(true) 하고 나면 바인딩 된 키 자체가 입력되지 않음. Tick에서 검토하여 해제하는 로직 추가
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
    else if (WasInputKeyJustReleased(EKeys::AnyKey)) // 예비 라인 : 키 입력이 풀리지 않았을 경우
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

// FIXME : Pause 상태에서도 인풋을 받을 수 있도록 수정
void AProfilingPlayerController::HandleProfilingPause()
{
    if (!bIsInputPressed)
    {
        bIsInputPressed = true;
        CurrentAction = EInputAction::ProfilingPause;
        bIsPaused = !bIsPaused;
        UGameplayStatics::SetGamePaused(GetWorld(), bIsPaused);

        // TODO : 코드 검토 후 디버그 블록 삭제
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
