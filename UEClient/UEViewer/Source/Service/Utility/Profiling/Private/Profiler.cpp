// Fill out your copyright notice in the Description page of Project Settings.


#include "Profiler.h"
#include "ProfilingPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"


UProfiler::UProfiler()

{
}

void UProfiler::SetProfilingStat(AProfilingPlayerController* PC)
{

	TArray<TObjectPtr<UUserWidget>> AllWidgets;

	UWorld* World = PC->GetWorld();
	if (!World)
	{
		return;
	}

	switch (PC->CurrentAction)
	{
	case EInputAction::ProfilingStart:
		if (!bIsStatToggled)
		{
			ExecuteConsoleCommand(PC, TEXT("stat fps"));
			ExecuteConsoleCommand(PC, TEXT("stat unit"));
			ExecuteConsoleCommand(PC, TEXT("stat unitgraph"));
			ExecuteConsoleCommand(PC, TEXT("t.maxfps 999"));
			bIsStatToggled = true;
		}
		else
		{
			ExecuteConsoleCommand(PC, TEXT("stat none"));
			bIsStatToggled = false;
		}

		break;

	case EInputAction::ProfilingSwitch:
		bIsStatToggled = false;

		if (CurrentThread == EThread::None)
		{
			ExecuteConsoleCommand(PC, TEXT("stat game"));
			CurrentThread = EThread::GameThread;
		}
		else if (CurrentThread == EThread::GameThread)
		{
			ExecuteConsoleCommand(PC, TEXT("stat game"));
			ExecuteConsoleCommand(PC, TEXT("stat SceneRendering"));
			CurrentThread = EThread::RenderingThread;
		}
		else if (CurrentThread == EThread::RenderingThread)
		{
			ExecuteConsoleCommand(PC, TEXT("stat SceneRendering"));
			ExecuteConsoleCommand(PC, TEXT("stat GPU"));
			CurrentThread = EThread::GPU;
		}
		else
		{
			ExecuteConsoleCommand(PC, TEXT("stat none"));
			ExecuteConsoleCommand(PC, TEXT("stat fps"));
			ExecuteConsoleCommand(PC, TEXT("stat unit"));
			ExecuteConsoleCommand(PC, TEXT("stat unitgraph"));
			CurrentThread = EThread::None;
		}

		break;

	case EInputAction::ProfilingSlow:
		if (!bIsStatToggled)
		{
			ExecuteConsoleCommand(PC, TEXT("stat none"));
			ExecuteConsoleCommand(PC, TEXT("stat fps"));
			ExecuteConsoleCommand(PC, TEXT("stat unit"));
			ExecuteConsoleCommand(PC, TEXT("stat unitgraph"));
			ExecuteConsoleCommand(PC, TEXT("stat slow"));
			bIsStatToggled = true;
		}
		else
		{
			ExecuteConsoleCommand(PC, TEXT("stat slow"));
			bIsStatToggled = false;
		}

		break;

	case EInputAction::ProfilingShowFlag:
		bIsStatToggled = false;

		if (CurrentFlag == EFlag::None)
		{
			CurrentFlag = EFlag::SkeletalMesh;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Only SkeletalMeshes are hidden")));
		}
		else if (CurrentFlag == EFlag::SkeletalMesh)
		{
			CurrentFlag = EFlag::StaticMesh;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Only StaticMeshes are hidden")));
		}
		else if (CurrentFlag == EFlag::StaticMesh)
		{
			CurrentFlag = EFlag::Particle;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Only Particle Flags are hidden")));
		}
		else if (CurrentFlag == EFlag::Particle)
		{
			CurrentFlag = EFlag::None;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString::Printf(TEXT("All Flags are Shown")));
		}

		ExecuteConsoleCommand(PC, FString::Printf(TEXT("showflag.skeletalmeshes %d"), CurrentFlag == EFlag::SkeletalMesh ? 0 : 1));
		ExecuteConsoleCommand(PC, FString::Printf(TEXT("showflag.staticmeshes %d"), CurrentFlag == EFlag::StaticMesh ? 0 : 1));
		ExecuteConsoleCommand(PC, FString::Printf(TEXT("showflag.Particles %d"), CurrentFlag == EFlag::Particle ? 0 : 1));

		break;

	case EInputAction::ProfilingShowFlagWidget:

		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(World, AllWidgets, UUserWidget::StaticClass(), true);
		for (UUserWidget* Widget : AllWidgets)
		{
			if (Widget->IsInViewport())
			{
				Widget->SetVisibility(bIsStatToggled ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
			}
		}

		if (!bIsStatToggled)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::Printf(TEXT("Widget ShowFlag = true")));
			bIsStatToggled = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("Widget ShowFlag = false")));
			bIsStatToggled = false;
		}


		break;

	case EInputAction::ProfilingRecord:
		if (!bIsRecording)
		{
			ExecuteConsoleCommand(PC, TEXT("stat startfile"));
			bIsRecording = true;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, FString::Printf(TEXT("Start Recording Profiling")));
		}
		else
		{
			ExecuteConsoleCommand(PC, TEXT("stat stopfile"));
			bIsRecording = false;
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, FString::Printf(TEXT("Stop Recording Profiling")));
		}

		break;

	default:
		// 기본 처리 (선택 사항)
		ExecuteConsoleCommand(PC, TEXT("stat none"));
		bIsStatToggled = false;
		UE_LOG(LogTemp, Warning, TEXT("Unknown action at Profiler"));
		break;

	}

}



void UProfiler::ExecuteConsoleCommand(APlayerController* PC, const FString Command)
{
	if (PC)
	{
		PC->ConsoleCommand(Command);
	}
}
