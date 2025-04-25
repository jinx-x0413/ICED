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
	GetHttpRequest(GetCartURL, [](FHttpResponsePtr Response, bool bSuccess)
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

		FCart Cart;
		FCartResponse CartResponse;

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
				//CartDataDelivery.Broadcast(CartResponse);
			}

		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("JSON 파싱 실패. JsonStr: %s"), *ContentString);
		}

	});
}

FCartResponse UGetCartInterface::GetCartStruct()
{
	return CartResponse;
}
