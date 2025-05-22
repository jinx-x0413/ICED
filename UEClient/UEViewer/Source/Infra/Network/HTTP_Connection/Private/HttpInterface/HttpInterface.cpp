// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpInterface/HttpInterface.h"

// Add default functionality here for any IHttpInterface functions that are not pure virtual.

void IHttpInterface::Start()
{
	if (!IsValid(Manager))
	{
		UE_LOG(LogTemp, Warning, TEXT("Manager is invalid at IHttpInterface::Start()"));
		return;
	}

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
	
	FString Token = TEXT("Bearer ") + ULogManager::GetLogManager()->GetToken();
	//FString Token = TEXT("Bearer eyJhbGciOiJIUzI1NiJ9.eyJjYXRlZ29yeSI6IkF1dGhvcml6YXRpb24iLCJ1c2VyaWQiOiJhZG1pbiIsInJvbGUiOiJST0xFX0FETUlOIiwiaWF0IjoxNzQ3ODkwMjY1LCJleHAiOjE3NDc5MDEwNjV9.txF4AVGO7iCuUrx77C60xgt6VdKGMYwFVwrq6h7YEkI");

	// Parse URL
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("GET");
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	Request->SetHeader(TEXT("Authorization"), Token);
	Request->SetHeader(TEXT("ngrok-skip-browser-warning"), TEXT("true"));

	//  &AWebApi::GetDataCallBack 부분 변경 (서버에서 받아온 Json 파싱 함수)
	// OnComplete를 안전하게 이동
	TFunction<void(FHttpResponsePtr, bool)> LocalOnComplete = MoveTemp(OnComplete);
	
	// ProcessRequest 전에 안전하게 람다 바인딩
	Request->OnProcessRequestComplete().BindLambda([LocalOnComplete = MoveTemp(LocalOnComplete)](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful) mutable
	{
		if (LocalOnComplete)
		{
			LocalOnComplete(Response, bWasSuccessful);
		}
	});

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
