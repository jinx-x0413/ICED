// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequest.h"
#include "HttpDependency.h"

UHttpRequest::UHttpRequest()
{
}

UHttpRequest::~UHttpRequest()
{
}

void UHttpRequest::SendUserDataHttpRequest()
{
	UE_LOG(LogTemp, Warning, TEXT("SendUserDataHttpRequest is run"));
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserHttpRequest = FHttpModule::Get().CreateRequest();
	UserHttpRequest->SetURL("https://randomuser.me/api/");
	UserHttpRequest->SetVerb("GET");

	//  &AWebApi::GetDataCallBack �κ� ���� (�������� �޾ƿ� Json �Ľ� �Լ�)
	UserHttpRequest->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::GetUserDataCallBack);

	// ��û ����
	if (!UserHttpRequest->ProcessRequest())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed"));
	}
}

void UHttpRequest::GetUserDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	FOpenApiTest OpenApi;
	FString OpenApiGender;

	// ������ ���������� ����� �Ϸ�Ǿ����� �˻�
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Response Failed."));
		return;
	}

	FString ContentString = Response->GetContentAsString();

	//Json �����͸� �����ϱ� ���� �迭
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ContentString);

	if (FJsonSerializer::Deserialize(Reader, JsonArray))
	{
		UE_LOG(LogTemp, Log, TEXT("JSON  Parse Successed: %s"), *ContentString);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON Parse Failed: %s"), *ContentString);
	}

	for (TSharedPtr<FJsonValue> RootData : JsonArray)
	{
		RootData.Get()->AsObject()->TryGetStringField(TEXT("gender"), OpenApiGender);
	}

	UE_LOG(LogTemp, Warning, TEXT("gender : %s"), *OpenApiGender);
}
