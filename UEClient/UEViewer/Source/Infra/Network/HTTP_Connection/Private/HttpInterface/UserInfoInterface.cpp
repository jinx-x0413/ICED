// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpInterface/UserInfoInterface.h"

void UUserInfoInterface::Start()
{
	IHttpInterface::Start();

	FString UserInfoURL = IHttpInterface::GetURL("UserInfo");

	if (UserInfoURL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("URL is missing in the JSON file"));
		return;
	}

	// 인터페이스의 헬퍼 함수 호출
	GetHttpRequest(UserInfoURL, [this](FHttpResponsePtr Response, bool bSuccess)
	{
		if (bSuccess && Response.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("Response: %s"), *Response->GetContentAsString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed"));
		}

		FString ContentString = Response->GetContentAsString();
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ContentString);

		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			Manager->UserInfo.userid = JsonObject->GetStringField("userid");
			Manager->UserInfo.email = JsonObject->GetStringField("email");
			Manager->UserInfo.username = JsonObject->GetStringField("username");

			UE_LOG(LogTemp, Warning, TEXT("UserId: %s"), *Manager->UserInfo.userid);
			UE_LOG(LogTemp, Warning, TEXT("Email: %s"), *Manager->UserInfo.email);
			UE_LOG(LogTemp, Warning, TEXT("Username: %s"), *Manager->UserInfo.username);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON."));
		}

	});
}
