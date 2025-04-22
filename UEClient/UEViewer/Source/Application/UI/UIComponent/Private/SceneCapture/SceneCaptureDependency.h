// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Engine/TextureRenderTarget2D.h"        // UTextureRenderTarget2D 사용
#include "Components/SceneCaptureComponent2D.h"  // SceneCaptureComponent2D 사용
#include "Components/SkeletalMeshComponent.h"    // USkeletalMeshComponent 사용
#include "Kismet/KismetRenderingLibrary.h"       // RenderTargetCreateStaticTexture2D 함수 사용
#include "Engine/Texture2D.h"                    // UTexture2D 사용
#include "Components/Image.h"


#include "SceneCaptureDependency.generated.h"

/**
 * 
 */
UCLASS()
class USceneCaptureDependency : public UObject
{
	GENERATED_BODY()
	
};
