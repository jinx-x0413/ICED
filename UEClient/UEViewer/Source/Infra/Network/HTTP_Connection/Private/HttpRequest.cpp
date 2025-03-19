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
	// ������ ���������� ����� �Ϸ�Ǿ����� �˻�
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Response Failed."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("GetUserDataCallBack"));

	FString ContentString = Response->GetContentAsString();

	UE_LOG(LogTemp, Warning, TEXT("Response Content: %s"), *ContentString);

	//Json �����͸� �����ϱ� ���� �迭
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ContentString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		UE_LOG(LogTemp, Warning, TEXT("JSON  Parse Successed: %s"), *ContentString);

		TArray<TSharedPtr<FJsonValue>> ResultsArray = JsonObject->GetArrayField(TEXT("results"));
		if (ResultsArray.Num() > 0)
		{
			TSharedPtr<FJsonObject> UserObject = ResultsArray[0]->AsObject();
			FOpenApiTest OpenApi;

			OpenApi.Gender = UserObject->GetStringField(TEXT("gender"));

			UserDataDelivery.Broadcast(OpenApi.Gender);

			UE_LOG(LogTemp, Warning, TEXT("gender : %s"), *OpenApi.Gender);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON Parse Failed: %s"), *ContentString);
	}

	
	

}
