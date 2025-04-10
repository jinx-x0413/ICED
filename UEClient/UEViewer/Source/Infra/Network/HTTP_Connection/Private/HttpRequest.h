// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"              // FHttpModule, FHttpRequestPtr, FHttpResponsePtr 등 HTTP 기능
#include "HttpModule.h"        // HTTP 요청을 보내기 위한 모듈
#include "Interfaces/IHttpRequest.h"   // HTTP 요청 인터페이스
#include "Interfaces/IHttpResponse.h"  // HTTP 응답 인터페이스
#include "HttpRequest.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FOpenApiTest
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString URL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Gender;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UserName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString ObjectName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString FilePath;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUserDataDelivery, const FOpenApiTest&, ApiTest);




UCLASS()
class UHttpRequest : public UObject
{
	GENERATED_BODY()
	
public:
	UHttpRequest();
	virtual ~UHttpRequest();

	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FUserDataDelivery UserDataDelivery;

	UFUNCTION(BlueprintCallable)
	virtual void SendUserDataHttpRequest();

private:
	void GetUserDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);




	// login
private:
	FOpenApiTest GetURLFromConfig();  // JSON 파일에서 URL 값을 가져오는 함수
	FOpenApiTest OpenApi;
};
