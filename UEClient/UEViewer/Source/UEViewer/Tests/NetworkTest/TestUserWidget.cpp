// Fill out your copyright notice in the Description page of Project Settings.


#include "TestUserWidget.h"
#include "Infra/Network/HTTP_Connection/private/HttpRequest.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"



void UTestUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // HttpRequestInstance가 유효한지 확인 후 델리게이트 바인딩
    if (HttpRequestInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Binding UpdateUI to UserDataDelivery"));

        HttpRequestInstance->UserDataDelivery.AddDynamic(this, &UTestUserWidget::UpdateUI);
    }
}

void UTestUserWidget::UpdateUI(const FString& Gender)
{
    UE_LOG(LogTemp, Warning, TEXT("Updating UI with Gender: %s"), *Gender);

    // GenderTextBlock이 바인딩되어 있는지 확인하고 텍스트 설정
    if (GenderTextBlock)
    {
        GenderTextBlock->SetText(FText::FromString(Gender));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GenderTextBlock is null"));
    }
}

