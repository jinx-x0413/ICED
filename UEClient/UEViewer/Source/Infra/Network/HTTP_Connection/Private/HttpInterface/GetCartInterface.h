// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HttpInterface.h"

#include "GetCartInterface.generated.h"



//USTRUCT(BlueprintType)
//struct FCart
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int32 id;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int32 fileId;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FString fileName;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FString description;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FString thumbnailUri;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FString size;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FString addedAt;
//};

// 루트 구조체
//USTRUCT(BlueprintType)
//struct FCartResponse
//{
//	GENERATED_USTRUCT_BODY()
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	FString userId;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	TArray<FCart> CartArray;
//
//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
//	int32 CartCount;
//};

/**
 * 
 */
UCLASS()
class UGetCartInterface : public UObject, public IHttpInterface
{
	GENERATED_BODY()

public:
	

	virtual void Start() override;

	

};
