// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/AssetContentTimebarWidget.h"
#include "Application/UI/Timebar/Public/Timebar.h"

void UAssetContentTimebarWidget::NativeConstruct()
{
	Super::NativeConstruct();

#if UE_BUILD_SHIPPING
	FString LogString = TEXT("Attempting AddDynamic to OnComponentListItemCreated");
	FFileHelper::SaveStringToFile(LogString, *(FPaths::ProjectLogDir() + TEXT("ShippingLog.txt")), FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
	if (UTimebarPlayer::GetTimebarPlayer())
	{
		if (!UTimebarPlayer::GetTimebarPlayer()->OnComponentListItemCreated.IsAlreadyBound(this, &UAssetContentTimebarWidget::OnComponentListItemCreated))
		{
			UTimebarPlayer::GetTimebarPlayer()->OnComponentListItemCreated.AddDynamic(this, &UAssetContentTimebarWidget::OnComponentListItemCreated);

#if UE_BUILD_SHIPPING
			FFileHelper::SaveStringToFile(TEXT("AddDynamic Done"), *(FPaths::ProjectLogDir() + TEXT("ShippingLog.txt")), FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
		}

		if (!UTimebarPlayer::GetTimebarPlayer()->OnComponentListDeleted.IsAlreadyBound(this, &UAssetContentTimebarWidget::OnComponentListDeleted))
		{
			UTimebarPlayer::GetTimebarPlayer()->OnComponentListDeleted.AddDynamic(this, &UAssetContentTimebarWidget::OnComponentListDeleted);
		}
	}

}

void UAssetContentTimebarWidget::NativeDestruct()
{
	Super::NativeDestruct();

	if (UTimebarPlayer::GetTimebarPlayer())
	{
		if (UTimebarPlayer::GetTimebarPlayer()->OnComponentListItemCreated.IsAlreadyBound(this, &UAssetContentTimebarWidget::OnComponentListItemCreated))
		{
			UTimebarPlayer::GetTimebarPlayer()->OnComponentListItemCreated.RemoveDynamic(this, &UAssetContentTimebarWidget::OnComponentListItemCreated);
		}

		if (UTimebarPlayer::GetTimebarPlayer()->OnComponentListDeleted.IsAlreadyBound(this, &UAssetContentTimebarWidget::OnComponentListDeleted))
		{
			UTimebarPlayer::GetTimebarPlayer()->OnComponentListDeleted.RemoveDynamic(this, &UAssetContentTimebarWidget::OnComponentListDeleted);
		}
	}
}

void UAssetContentTimebarWidget::OnComponentListItemCreated(UTrack* InTrack, UUserWidget* InSceneCaptureWidget)
{
#if UE_BUILD_SHIPPING
	FString LogString = FString::Printf(TEXT("OnComponentListItemCreated Called at UAssetContentTimebarWidget"));

	FString LogFilePath = FPaths::ProjectLogDir() + TEXT("ShippingLog.txt");

	// Append 모드로 여러 줄 저장 가능하게
	FFileHelper::SaveStringToFile(LogString, *LogFilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), FILEWRITE_Append);
#endif
	ExecOnComponentListItemCreated(InTrack, InSceneCaptureWidget);
}

void UAssetContentTimebarWidget::OnComponentListDeleted()
{
	ExecOnComponentListDeleted();
}
