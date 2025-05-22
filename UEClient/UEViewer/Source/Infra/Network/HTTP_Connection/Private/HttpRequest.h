// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"              // FHttpModule, FHttpRequestPtr, FHttpResponsePtr 등 HTTP 기능
#include "HttpModule.h"        // HTTP 요청을 보내기 위한 모듈
#include "Interfaces/IHttpRequest.h"   // HTTP 요청 인터페이스
#include "Interfaces/IHttpResponse.h"  // HTTP 응답 인터페이스

#include "HttpRequest.generated.h"


class IHttpInterface;

/**
 * 
 */


USTRUCT(BlueprintType)
struct FUserInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString userid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString email;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString username;
};


USTRUCT(BlueprintType)
struct FCart
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 fileId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString fileName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString description;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString thumbnailUri;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString size;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString addedAt;
};

// 루트 구조체
USTRUCT(BlueprintType)
struct FCartResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString userId;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FCart> CartArray;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CartCount;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyGltfAssetActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetCartData, FCartResponse, InCartData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetUserData, FUserInfo, InUserData);

UENUM(BlueprintType)
enum class EApiType : uint8	//블루프린트에서 쓰려면 uint8 붙이셈
{
	BaseURL,
	GetCart,       // 장바구니 조회 API
	DownloadModel,  // 모델 다운로드 API
	UserInfo
};

UCLASS()
class UHttpRequest : public UObject
{
	GENERATED_BODY()
	
public:
	UHttpRequest();
	virtual ~UHttpRequest();

	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FDestroyGltfAssetActor DestroyGltfAssetActor;


private:
	UFUNCTION(BlueprintCallable)
	void DestroyActorDelegate();
	
	// login
private:

public:
	void StartHttp(); // sub (strategy)
	TScriptInterface<IHttpInterface> CurrentInterface;

	void CreateInterface(EApiType InApiType = EApiType::BaseURL); // sub (factory)

	// business logic
	UFUNCTION(BlueprintCallable)
	void SendHttp(EApiType InApiType = EApiType::BaseURL);

	//구조체 받는 함수를 하나를 만들어 델리게이트를 2개 만들고 데이터를 그 구조체로 줘



	// get data
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "HTTP")
	FOnGetCartData OnGetCartData;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "HTTP")
	FOnGetUserData OnGetUserData;

	UFUNCTION(BlueprintCallable)
	void GetData();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FUserInfo UserInfo;

	UPROPERTY(BluePrintReadWrite, EditAnywhere)	
	FCartResponse CartResponse;

	UPROPERTY(BluePrintReadWrite, EditAnywhere)	
	FCart Cart;

	UPROPERTY(BluePrintReadWrite, EditAnywhere)
	int32 CurrentCartId;
};
