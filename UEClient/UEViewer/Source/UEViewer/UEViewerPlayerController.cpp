// Fill out your copyright notice in the Description page of Project Settings.


#include "UEViewerPlayerController.h"

AUEViewerPlayerController::AUEViewerPlayerController()
{
}

AUEViewerPlayerController::~AUEViewerPlayerController()
{
}

void AUEViewerPlayerController::BeginPlay()
{
    Super::BeginPlay();

    if (UserWidgetClass)
    {
        // ���� ����
        UserWidgetInstance = CreateWidget<UTestUserWidget>(this, UserWidgetClass);
        if (UserWidgetInstance)
        {
            // ȭ�鿡 �߰�
            UserWidgetInstance->AddToViewport();

        }
    }

    
}

void AUEViewerPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("AUEViewerPlayerController is invalid!"));
        return;
    }

    if (!InputComponent)
    {
        UE_LOG(LogTemp, Error, TEXT("InputComponent is NULL in AUEViewerPlayerController!"));
        return;
    }

    // "1" Ű �Է� ���ε� (Pressed �� OnPressOneKey ȣ��)??
    InputComponent->BindAction("PressOne", IE_Pressed, this, &AUEViewerPlayerController::OnPressOneKey);

}

void AUEViewerPlayerController::OnPressOneKey()
{
   if (!IsValid(this))
    {
        UE_LOG(LogTemp, Error, TEXT("AUEViewerPlayerController is invalid!"));
        return;
    }

    UE_LOG(LogTemp, Error, TEXT("Pressed 1 Key!"));

    UEntryPoint::SendUserDataHttpRequest();
}
