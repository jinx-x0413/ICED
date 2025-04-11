// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include "LogDependency.h"

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateLoginStatus();
}

void ULoginWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void ULoginWidget::UpdateLoginStatus()
{
	if (ULogManager::GetLogManager()->IsLoggedIn())
	{
		FString Token = ULogManager::GetLogManager()->GetToken();
		ExecUpdateLoginStatus(Token);
	}
	else
	{
		ExecUpdateLoginFailed();
		UE_LOG(LogTemp, Warning, TEXT("Login Failed - LoginWidget"));
	}
}
