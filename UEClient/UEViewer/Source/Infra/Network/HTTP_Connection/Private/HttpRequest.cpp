// Fill out your copyright notice in the Description page of Project Settings.


#include "HttpRequest.h"
#include "HttpDependency.h"
#include <JsonObjectConverter.h>
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "HttpInterface/InterfaceDependency.h"

UHttpRequest::UHttpRequest()
{
}

UHttpRequest::~UHttpRequest()
{
}

void UHttpRequest::DestroyActorDelegate()
{
	DestroyGltfAssetActor.Broadcast();
}

void UHttpRequest::StartHttp()
{
	CurrentInterface->Start();
}

void UHttpRequest::CreateInterface(EApiType InApiType)
{
	if (IsValid(CurrentInterface.GetObject()) && CurrentInterface.GetObject()->IsRooted())
	{
		CurrentInterface.GetObject()->RemoveFromRoot();
		CurrentInterface.GetObject()->MarkAsGarbage();
		CurrentInterface = nullptr;
	}

	switch (InApiType)
	{
	case EApiType::BaseURL:
		break;

	case EApiType::GetCart:
		CurrentInterface = NewObject<UGetCartInterface>();
		break;

	case EApiType::DownloadModel:
		CurrentInterface = NewObject<UDownloadInterface>();
		break;

	case EApiType::UserInfo:
		CurrentInterface = NewObject<UUserInfoInterface>();
		break;

	default:
		break;
	}

	CurrentInterface.GetObject()->AddToRoot();
	CurrentInterface->Manager = this;
}

void UHttpRequest::SendHttp(EApiType InApiType)
{
	CreateInterface(InApiType);
	StartHttp();
}

void UHttpRequest::GetData()
{
	if (Cast<UGetCartInterface>(CurrentInterface.GetObject()))
	{
		OnGetCartData.Broadcast(CartResponse);
	}
	else if (Cast<UDownloadInterface>(CurrentInterface.GetObject()))
	{
		
	}
	else if (Cast<UUserInfoInterface>(CurrentInterface.GetObject()))
	{
		OnGetUserData.Broadcast(UserInfo);
	}
	
}
