// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TestUserWidget.generated.h"


class UTextBlock;
class UHttpRequest;  // HTTP 요청 클래스
class UButton;

/**
 * 
 */
UCLASS()
class UEVIEWER_API UTestUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable)
    void UpdateUI(const FString& Gender);  // UI 업데이트 함수

    // 바인딩할 텍스트 블록 (디자이너에서 변수로 지정해야 함)
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* GenderTextBlock;

    UPROPERTY()
    UHttpRequest* HttpRequestInstance;  // HTTP 요청 객체
};
