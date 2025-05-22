// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpInterface/GetCartInterface.h"

void UGetCartInterface::Start()
{
	IHttpInterface::Start();

	FString GetCartURL = IHttpInterface::GetURL("GetCart");

	if (GetCartURL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("URL is missing in the JSON file"));
		return;
	}

	// 인터페이스의 헬퍼 함수 호출
	GetHttpRequest(GetCartURL, [this](FHttpResponsePtr Response, bool bSuccess)
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

		//Json 데이터를 저장하기 위한 배열
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<TCHAR>> Reader = TJsonReaderFactory<TCHAR>::Create(ContentString);

		
		if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
		{
			
			Manager->CartResponse.CartArray.Empty();

			//UE_LOG(LogTemp, Warning, TEXT("UserId: %s, CartCount: %d"), *DataStructOwner->CartResponse.userId, DataStructOwner->CartResponse.CartCount);

			int cartarraycount = 0;
			const TArray<TSharedPtr<FJsonValue>>* ItemsArray = nullptr;
			if (JsonObject->TryGetArrayField("items", ItemsArray))
			{
				for (auto& ItemValue : *ItemsArray)
				{
					TSharedPtr<FJsonObject> ItemObj = ItemValue.Get()->AsObject();

					Manager->Cart.id = ItemObj->GetIntegerField("id");
					Manager->Cart.fileName = ItemObj->GetStringField("fileName");
					Manager->Cart.fileId = ItemObj->GetIntegerField("fileId");
					Manager->Cart.size = ItemObj->GetStringField("size");
					Manager->Cart.description = ItemObj->GetStringField("description");
					Manager->Cart.thumbnailUri = ItemObj->GetStringField("thumbnailUri");
					Manager->Cart.addedAt = ItemObj->GetStringField("addedAt");
					Manager->CartResponse.CartArray.Add(Manager->Cart);
				}
				
			}

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JSON 파싱 실패. JsonStr: %s"), *ContentString);
		}

	});
}

