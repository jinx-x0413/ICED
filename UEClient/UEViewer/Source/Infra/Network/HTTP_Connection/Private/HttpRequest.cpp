// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequest.h"
#include "HttpDependency.h"
#include <JsonObjectConverter.h>
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"

UHttpRequest::UHttpRequest()
{
}

UHttpRequest::~UHttpRequest()
{
}

void UHttpRequest::SendUserDataHttpRequest()	//파라미터 넣어서 GetURL에 어떤 URL 받을건지 하는 형식으로 해야함
{
	UE_LOG(LogTemp, Warning, TEXT("SendUserDataHttpRequest is run"));

	GetURLFromConfig();
	// get login URL from JSON
	FString GetCartURL = GetURL("GetCart");
	FString DownloadModelURL = GetURL("DownloadModel");
	if (GetCartURL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("URL is missing in the JSON file"));
		return;
	}

	// Parse URL
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> UserHttpRequest = FHttpModule::Get().CreateRequest();
	UserHttpRequest->SetURL(DownloadModelURL);
	UserHttpRequest->SetVerb("GET");
	UserHttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	UserHttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer eyJhbGciOiJIUzI1NiJ9.eyJjYXRlZ29yeSI6IkF1dGhvcml6YXRpb24iLCJ1c2VyaWQiOiJhZG1pbiIsInJvbGUiOiJST0xFX0FETUlOIiwiaWF0IjoxNzQ1Mzk3ODU4LCJleHAiOjE3NDU0MDg2NTh9.PE6ZQP0Ub1RUXuI0IrW3PHrKnK1NzTOdAFzu1zIpp8U"));
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


	//////////////////////////////
	if (bWasSuccessful && Response.IsValid() && Response->GetResponseCode() == 200)
	{
		// 저장 경로: Saved/DownloadedModels/
		FString FilePath = FPaths::ProjectSavedDir() / TEXT("DownloadedModels/MyModel.glb");

		// 디렉토리 없으면 생성
		IFileManager::Get().MakeDirectory(*FPaths::GetPath(FilePath), true);

		// 저장
		if (FFileHelper::SaveArrayToFile(Response->GetContent(), *FilePath))
		{
			UE_LOG(LogTemp, Log, TEXT("GLTF file saved to: %s"), *FilePath);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save GLTF file."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HTTP request failed or invalid response."));
	}
	/////////////////////////////

	//Json 데이터를 저장하기 위한 배열
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ContentString);
	
	////////////////////////////////////////////////////////////////////////////////////// real api parsing
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		CartResponse.CartArray.Empty();

		UE_LOG(LogTemp, Warning, TEXT("UserId: %s, CartCount: %d"), *CartResponse.userId, CartResponse.CartCount);
		int cartarraycount = 0;
		const TArray<TSharedPtr<FJsonValue>>* ItemsArray = nullptr;
		if (JsonObject->TryGetArrayField("items", ItemsArray))
		{
			for (auto& ItemValue : *ItemsArray)
			{
				TSharedPtr<FJsonObject> ItemObj = ItemValue.Get()->AsObject();
				
				Cart.fileName = ItemObj->GetStringField("fileName");
				Cart.fileId = ItemObj->GetIntegerField("fileId");
				Cart.size = ItemObj->GetStringField("size");
				Cart.description = ItemObj->GetStringField("description");
				Cart.thumbnailUri = ItemObj->GetStringField("thumbnailUri");
				Cart.addedAt = ItemObj->GetStringField("addedAt");
				CartResponse.CartArray.Add(Cart);
				UE_LOG(LogTemp, Warning, TEXT("fileName: %s, fileId: %d, size: %s, description: %s, thumbnailUri: %s, AddedAt: %s"), *Cart.fileName, Cart.fileId, *Cart.size, *Cart.description, *Cart.thumbnailUri, *Cart.addedAt);
			}
			for (auto& CarArrayCount : CartResponse.CartArray)
			{
				cartarraycount++;
			}
			UE_LOG(LogTemp, Warning, TEXT("CartArrayCount : %d"), cartarraycount);
			cartarraycount = 0;
			CartDataDelivery.Broadcast(CartResponse);
			CartResponse.CartArray.Empty();
		}
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON 파싱 실패. JsonStr: %s"), *ContentString);
	}
	//////////////////////////////////////////////////////////////////////////////////////	sample api parsing
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
		UserDataDelivery.Broadcast(OpenApi);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("JSON Parse Failed: %s"), *ContentString);
	}

}

FString UHttpRequest::GetURL(const FString& APIType)
{
	if (URLMap.Contains(APIType))
	{
		return URLMap[APIType];
	}

	return TEXT("APIType is InValid");
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
