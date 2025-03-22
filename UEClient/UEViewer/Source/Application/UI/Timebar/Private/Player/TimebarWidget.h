// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "TimebarWidget.generated.h"

/**
 * 
 */
UCLASS()
class UTimebarWidget : public UUserWidget
{
	GENERATED_BODY()
	


	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;



	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;


	
public:
	UFUNCTION()
	void Run(float InCurrentTime);

	UFUNCTION(BlueprintImplementableEvent)
	void ExecRun(float InCurrentTime, float InConvertedValue);

	UFUNCTION(BlueprintCallable)
	void SetScrollOffset(UScrollBox* InScrollBox, float InCurrentTime);
};
