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
			return Module->HttpRequest->SendUserDataHttpRequest();
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
