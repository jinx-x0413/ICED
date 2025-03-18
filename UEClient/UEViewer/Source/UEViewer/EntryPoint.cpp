// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryPoint.h"

void UEntryPoint::LoadActor()
{

}

void UEntryPoint::SendUserDataHttpRequest()
{
	if (FModuleManager::Get().IsModuleLoaded(TEXT("HTTP_Connection")))
	{
		FHTTP_Connection* Module = FModuleManager::Get().GetModulePtr<FHTTP_Connection>("HTTP_Connection");
		if (Module)
		{
			if (Module->HttpRequest) // NULL 체크 추가
			{
				return Module->HttpRequest->SendUserDataHttpRequest();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("HttpRequest is NULL!"));
			}
		}
	}
}

//void UEntryPoint::SendUserDataHttpRequest()
//{
//	if (FModuleManager::Get().IsModuleLoaded(TEXT("HTTP_Connection")))
//	{
//		FHTTP_Connection* Module = FModuleManager::Get().GetModulePtr<FHTTP_Connection>("HTTP_Connection");
//		if (Module)
//		{
//			return Module->HttpRequest->SendUserDataHttpRequest();
//		}
//	}
//}
