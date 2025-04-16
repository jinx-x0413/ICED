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
	FString URL = GetURLFromConfig().URL;
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
			

			OpenApi.Gender = UserObject->GetStringField(TEXT("gender"));

			

			UE_LOG(LogTemp, Warning, TEXT("gender : %s"), *OpenApi.Gender);
		}

		const TArray<TSharedPtr<FJsonValue>>* Users;
		if (JsonObject->TryGetArrayField("User", Users))
		{
			
			for (auto& UserValue : *Users)
			{
				TSharedPtr<FJsonObject> UserObject = UserValue->AsObject();
				OpenApi.UserName = UserObject->GetStringField("UserName");
				UE_LOG(LogTemp, Warning, TEXT("UserName: %s"), *OpenApi.UserName);
			}
		}

		// Objects 배열 파싱
		const TArray<TSharedPtr<FJsonValue>>* Objects;
		if (JsonObject->TryGetArrayField("Objects", Objects))
		{
			for (auto& ObjectValue : *Objects)
			{
				TSharedPtr<FJsonObject> ObjectObject = ObjectValue->AsObject();
				OpenApi.ObjectName = ObjectObject->GetStringField("ObjectName");
				UE_LOG(LogTemp, Warning, TEXT("ObjectName: %s"), *OpenApi.ObjectName);

				TSharedPtr<FJsonObject> FilePath = ObjectValue->AsObject();
				OpenApi.FilePath = ObjectObject->GetStringField("FilePath");
				UE_LOG(LogTemp, Warning, TEXT("FilePath: %s"), *OpenApi.FilePath);
			}
		}

		UserDataDelivery.Broadcast(OpenApi);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON Parse Failed: %s"), *ContentString);
	}

	
	

}

FOpenApiTest UHttpRequest::GetURLFromConfig()
{
	FString ProjectFilePath = FPaths::ProjectDir() + TEXT("/Settings/LoginSetting.json");
	FString JsonRaw;

	if (!FFileHelper::LoadFileToString(JsonRaw, *ProjectFilePath))
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to load JSON file: %s"), *ProjectFilePath);
		return OpenApi;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(JsonRaw);
	

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		OpenApi.URL = JsonObject->GetStringField(TEXT("URL"));
		UE_LOG(LogTemp, Warning, TEXT("URL from JSON: %s"), *OpenApi.URL);

		//UserName
		const TArray<TSharedPtr<FJsonValue>>* Users;
		if (JsonObject->TryGetArrayField("User", Users))
		{
			for (auto& UserValue : *Users)
			{
				//UserName
				TSharedPtr<FJsonObject> UserObject = UserValue->AsObject();
				OpenApi.UserName = UserObject->GetStringField("UserName");
				UE_LOG(LogTemp, Warning, TEXT("UserName: %s"), *OpenApi.UserName);

				//Email
				TSharedPtr<FJsonObject> Email = UserValue->AsObject();
				OpenApi.Email = UserObject->GetStringField("Email");
				UE_LOG(LogTemp, Warning, TEXT("Email: %s"), *OpenApi.Email);
			}
		}
			
		// Objects 배열 파싱
		const TArray<TSharedPtr<FJsonValue>>* Objects;
		if (JsonObject->TryGetArrayField("Objects", Objects))
		{
			for (auto& ObjectValue : *Objects)
			{
				//ObjectName
				TSharedPtr<FJsonObject> ObjectObject = ObjectValue->AsObject();
				OpenApi.ObjectName = ObjectObject->GetStringField("ObjectName");
				UE_LOG(LogTemp, Warning, TEXT("ObjectName: %s"), *OpenApi.ObjectName);

				//FilePath
				TSharedPtr<FJsonObject> FilePath = ObjectValue->AsObject();
				OpenApi.FilePath = ObjectObject->GetStringField("FilePath");
				UE_LOG(LogTemp, Warning, TEXT("FilePath: %s"), *OpenApi.FilePath);
			}
		}

		return OpenApi;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON file."));
		return OpenApi;
	}

}
