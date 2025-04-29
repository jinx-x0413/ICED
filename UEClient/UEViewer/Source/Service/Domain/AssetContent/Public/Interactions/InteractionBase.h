// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Application/UI/Timebar/Private/Player/Clip.h"
#include "InteractionBase.generated.h"


UENUM(BlueprintType)
enum class EInteractionTriggerType : uint8
{
	AUTOSTART		UMETA(DisplayName = "AutoStart"),
	CLICKOBJECT		UMETA(DisplayName = "ClickObject"),
	PROXIMITY		UMETA(DisplayName = "Proximity")
};

UENUM(BlueprintType)
enum class EInteractionType : uint8
{
	NONE			UMETA(DisplayName = "None"),
	CAMERA			UMETA(DisplayName = "Camera"),
	HIDDEN			UMETA(DisplayName = "Hidden"),
	HIGHLIGHT		UMETA(DisplayName = "Highlight"),
	TRANSFORM		UMETA(DisplayName = "Transform"),
	POPUP			UMETA(DisplayName = "Popup"),
	COLOR			UMETA(DisplayName = "Color")
};

UENUM(BlueprintType)
enum class ETransformInteractionType : uint8
{
	LOCATION,
	ROTATION,
	SCALE
};
UENUM(BlueprintType)
enum class ETransformInteractionDirection : uint8
{
	AUTO				UMETA(DisplayName = "Auto"),
	UP					UMETA(DisplayName = "Up"),
	DOWN				UMETA(DisplayName = "Down"),
	LEFT				UMETA(DisplayName = "Left"),
	RIGHT				UMETA(DisplayName = "Right")
};


USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Index;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Name;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UInteractionBase> TargetClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* TargetActor;

	// Camera Interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform TargetTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TargetArmLength;
	UPROPERTY(BLueprintReadWrite, EditAnywhere)
	bool bUseCustomCameraTransform;

	// Hidden Interaction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHidden;

	// HighlightInteraction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsHighlighted;

	// TransformInteraction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent* TargetComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform StartTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FTransform EndTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ETransformInteractionDirection TransformDirection;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseCustomEndTransform;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsReversed;

	// PopupInteraction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UUserWidget* TargetPopupWidget;

	// ColorInteraction
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* BaseMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FLinearColor MaterialBaseColor;

	FInteractionData()
		: Index(0)
		, Name(TEXT(""))

		, TargetActor(nullptr)

		, TargetTransform()
		, TargetArmLength()
		, bUseCustomCameraTransform(false)

		, bIsHidden(false)

		, bIsHighlighted(false)

		, TargetComponent(nullptr)
		, StartTransform()
		, EndTransform()
		, TransformDirection(ETransformInteractionDirection::AUTO)
		, bUseCustomEndTransform(false)
		, bIsReversed(false)

		, TargetPopupWidget(nullptr)

		, BaseMaterial(nullptr)
		, MaterialBaseColor(FLinearColor(1, 1, 1, 1))
	{
	}
};

UCLASS(BlueprintType)
class ASSETCONTENT_API UInteractionBase : public UClip
{
	GENERATED_BODY()


	// Clip overriding
public:
	virtual void Play(float InCurrentTime) override;
	virtual void Pause() override;
	virtual void Stop() override;


	// Interaction
public:
	virtual void Initialize(FInteractionData InInteractionData);
	virtual void Start() {}
	virtual void Finish() {}
	virtual void Reset() {}


public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FInteractionData TargetData;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInteracting = false;

	FVector GetTargetActorCenter();
};
