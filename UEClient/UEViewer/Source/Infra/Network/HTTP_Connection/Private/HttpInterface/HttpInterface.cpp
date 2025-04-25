// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpInterface/HttpInterface.h"

// Add default functionality here for any IHttpInterface functions that are not pure virtual.

void IHttpInterface::Start()
{
	FString ProjectFilePath = FPaths::ProjectDir() + TEXT("/Settings/LoginSetting.json");
	FString JsonRaw;
	if (!FFileHelper::LoadFileToString(JsonRaw, *ProjectFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *ProjectFilePath);
		return;
	}
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		//API URL
		const TArray<TSharedPtr<FJsonValue>>* URLs;
		if (JsonObject->TryGetArrayField("URL", URLs))
		{
			for (auto& Entry : *URLs)
			{
				TSharedPtr<FJsonObject> Obj = Entry->AsObject();
				FString APIType = Obj->GetStringField("APIType");
				FString URL = Obj->GetStringField("URL");

				URLMap.Add(APIType, URL);
			}
		}
	}
}


void IHttpInterface::GetHttpRequest(const FString& URL, TFunction<void(FHttpResponsePtr, bool)> OnComplete) const
{
	// Parse URL
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer eyJhbGciOiJIUzI1NiJ9.eyJjYXRlZ29yeSI6IkF1dGhvcml6YXRpb24iLCJ1c2VyaWQiOiJhZG1pbiIsInJvbGUiOiJST0xFX0FETUlOIiwiaWF0IjoxNzQ1NTg1ODczLCJleHAiOjE3NDU1OTY2NzN9.PeZXPEEyudgt94SYbmNQ4zW9dkP_Dm5HhwXHIp5g8vI"));
	//  &AWebApi::GetDataCallBack 부분 변경 (서버에서 받아온 Json 파싱 함수)
	// 바인드 대신 람다 사용
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
	{
		OnComplete(Response, bWasSuccessful);
	});

	Request->ProcessRequest();

	// 요청 실행
	if (!Request->ProcessRequest())
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed"));
	}
}

FString IHttpInterface::GetURL(const FString& APIType)
{
	if (URLMap.Contains(APIType))
	{
		return URLMap[APIType];
	}
	return TEXT("APIType is InValid");
}
