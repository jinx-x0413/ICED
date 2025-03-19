// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "TestUserWidget.generated.h"


class UTextBlock;
class UHttpRequest;  // HTTP ��û Ŭ����
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
    void UpdateUI(const FString& Gender);  // UI ������Ʈ �Լ�

    // ���ε��� �ؽ�Ʈ ��� (�����̳ʿ��� ������ �����ؾ� ��)
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    UTextBlock* GenderTextBlock;

    UPROPERTY()
    UHttpRequest* HttpRequestInstance;  // HTTP ��û ��ü
};
