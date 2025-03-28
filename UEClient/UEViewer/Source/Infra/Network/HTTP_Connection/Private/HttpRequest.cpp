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
	
	// get login URL from JSON
	FString URL = GetURLFromConfig();
	if (URL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("URL is missing in the JSON file"));
		return;
	}

	// Parse URL
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserHttpRequest = FHttpModule::Get().CreateRequest();
	UserHttpRequest->SetURL(URL);
	UserHttpRequest->SetVerb("GET");

	//  &AWebApi::GetDataCallBack 부분 변경 (서버에서 받아온 Json 파싱 함수)
	UserHttpRequest->OnProcessRequestComplete().BindUObject(this, &UHttpRequest::GetUserDataCallBack);

	// 요청 실행
	if (!UserHttpRequest->ProcessRequest())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed"));
	}
}

void UHttpRequest::GetUserDataCallBack(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// 서버와 성공적으로 통신이 완료되었는지 검사
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Response Failed."));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("GetUserDataCallBack"));

	FString ContentString = Response->GetContentAsString();

	UE_LOG(LogTemp, Warning, TEXT("Response Content: %s"), *ContentString);

	//Json 데이터를 저장하기 위한 배열
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

FString UHttpRequest::GetURLFromConfig()
{
	FString FilePath = FPaths::ProjectDir() + TEXT("/Settings/LoginSetting.json");
	FString JsonRaw;

	if (!FFileHelper::LoadFileToString(JsonRaw, *FilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
		return TEXT("");
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		FString URL = JsonObject->GetStringField(TEXT("URL"));
		UE_LOG(LogTemp, Warning, TEXT("URL from JSON: %s"), *URL);
		return URL;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file."));
		return TEXT("");
	}

}
