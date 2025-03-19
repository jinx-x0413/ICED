// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EntryPoint.h"
#include "UEViewer/Tests/NetworkTest/TestUserWidget.h"
#include "UEViewerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UEVIEWER_API AUEViewerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AUEViewerPlayerController();
	virtual ~AUEViewerPlayerController();

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UTestUserWidget> UserWidgetClass;

	virtual void SetupInputComponent() override;

	void OnPressOneKey();

private:
	UTestUserWidget* UserWidgetInstance;
};
