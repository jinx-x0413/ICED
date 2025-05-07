// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpInterface/DownloadInterface.h"
#include "HttpInterface.h"
void UDownloadInterface::Start()
{
	IHttpInterface::Start();

	FString DownloadModelURL = IHttpInterface::GetURL("DownloadModel");

	if (DownloadModelURL.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("URL is missing in the JSON file"));
		return;
	}

    // 인터페이스의 헬퍼 함수 호출
    GetHttpRequest(DownloadModelURL, [this](FHttpResponsePtr Response, bool bSuccess)
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
		if (bSuccess && Response.IsValid() && Response->GetResponseCode() == 200)
		{
			// 저장 경로: Resource/
			UE_LOG(LogTemp, Warning, TEXT("filename = %s"), *Manager->Cart.fileName);
			FString FilePath = FPaths::ProjectDir() / TEXT("Resource/") + *Manager->Cart.fileName;

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
        
    });
	
}
