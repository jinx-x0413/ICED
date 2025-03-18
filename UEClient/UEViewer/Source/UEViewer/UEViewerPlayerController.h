// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EntryPoint.h"
#include "UEViewerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UEVIEWER_API AUEViewerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AUEViewerPlayerController();
	~AUEViewerPlayerController();

public:

	virtual void SetupInputComponent() override;

	void OnPressOneKey();
};
