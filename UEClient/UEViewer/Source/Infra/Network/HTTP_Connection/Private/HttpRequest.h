// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Http.h"              // FHttpModule, FHttpRequestPtr, FHttpResponsePtr �� HTTP ���
#include "HttpModule.h"        // HTTP ��û�� ������ ���� ���
#include "Interfaces/IHttpRequest.h"   // HTTP ��û �������̽�
#include "Interfaces/IHttpResponse.h"  // HTTP ���� �������̽�
#include "HttpRequest.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUserDataDelivery, const FString&, Gender);


USTRUCT(BlueprintType)
struct FOpenApiTest
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString Gender;

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

	UFUNCTION(BlueprintCallable)
	virtual void SendUserDataHttpRequest();

private:
	void GetUserDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
