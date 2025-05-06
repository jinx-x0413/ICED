// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class LOG_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;



	// feature
private:
	UFUNCTION()
	void UpdateLoginStatus();

public:
	UFUNCTION(BlueprintImplementableEvent)
	void ExecUpdateLoginStatus(const FString& InToken);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecUpdateLoginFailed();

};
