// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragDrop/TimebarDroppableWidget.h"
#include "TimeScaleWidget.generated.h"




UCLASS()
class UTimeScaleWidget : public UTimebarDroppableWidget
{
	GENERATED_BODY()


	// construct
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	bool bIsRulerDrawn = false;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	


	// drag
public:
	virtual FReply NativeOnMouseButtonDown(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent) override;


	bool bIsDragging = false;

	virtual FReply NativeOnMouseMove(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent
	) override;

	virtual FReply NativeOnMouseButtonUp(
		const FGeometry& InGeometry,
		const FPointerEvent& InMouseEvent
	) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;


	// play
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxScale;

	UFUNCTION()
	void SetMaxScale();

	UFUNCTION()
	void FinishPlay(float InCurrentTime);


	//// Scale
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentRulerSpacing;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RulerLength;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentZoomScale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UBorder*> RulerTickArray;

	UFUNCTION(BlueprintCallable)
	void DrawRuler();

	UFUNCTION(BlueprintCallable)
	void SetRulerSpacing(float InRulerSpacing);

	UFUNCTION(BlueprintCallable)
	void SetZoomScale(float InZoomScale);


	

};
