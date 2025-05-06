// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"              // FHttpModule, FHttpRequestPtr, FHttpResponsePtr 등 HTTP 기능
#include "HttpModule.h"        // HTTP 요청을 보내기 위한 모듈
#include "Interfaces/IHttpRequest.h"   // HTTP 요청 인터페이스
#include "Interfaces/IHttpResponse.h"  // HTTP 응답 인터페이스
#include "HttpInterface/GetCartInterface.h"

#include "HttpRequest.generated.h"


class IHttpInterface;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FOpenApiTest
{
	GENERATED_BODY()


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString URL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Gender;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UserName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Email;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ObjectName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString FilePath;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUserDataDelivery, const FOpenApiTest&, ApiTest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyGltfAssetActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetCartData, FCartResponse, InCartData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGetUserData, FOpenApiTest, InCartData);

UENUM(BlueprintType)
enum class EApiType : uint8	//블루프린트에서 쓰려면 uint8 붙이셈
{
	BaseURL,
	GetCart,       // 장바구니 조회 API
	DownloadModel  // 모델 다운로드 API
};

UCLASS()
class UHttpRequest : public UObject
{
	GENERATED_BODY()
	
public:
	UHttpRequest();
	virtual ~UHttpRequest();

	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FUserDataDelivery UserDataDelivery;

	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FDestroyGltfAssetActor DestroyGltfAssetActor;

	UFUNCTION(BlueprintCallable)
	virtual void SendUserDataHttpRequest();

private:
	void GetUserDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	FString GetURL(const FString& APIType);

	UFUNCTION(BlueprintCallable)
	void DestroyActorDelegate();
	
	// login
private:
	FOpenApiTest GetURLFromConfig();  // JSON 파일에서 URL 값을 가져오는 함수
	FOpenApiTest OpenApi;
	TMap<FString, FString> URLMap;

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
};
