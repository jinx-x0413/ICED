// Fill out your copyright notice in the Description page of Project Settings.


#include "TimebarController.h"
#include "TimebarDependency.h"

// Construct
UTimebarController::UTimebarController()
{
}

UTimebarController::~UTimebarController()
{
}

void UTimebarController::BeginDestroy()
{
	Shutdown();

	Super::BeginDestroy();
}

void UTimebarController::Shutdown()
{
	if (!ManagerMap.IsEmpty())
	{
		for (auto& Wrapper : ManagerMap)
		{
			if (UTimebarManager* Manager = Wrapper.Value.Manager)
			{
				if (IsValid(Manager) && Manager->IsRooted())
				{
					Manager->RemoveFromRoot();
					Manager->MarkAsGarbage();
				}

				Wrapper.Value.Manager = nullptr;
			}
		}

		ManagerMap.Empty();  // 포인터 null 처리 후 map 클리어
	}
}




// Business Logic
void UTimebarController::CreateTrack(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName)
{
	bool bIsSideTrack = InContentWidget->IsChildOf(USideTrackWidget::StaticClass());
	if (bIsSideTrack && UTimebarPlayer::GetTimebarPlayer()->SelectedTrack)
	{
		// create side track
		UTimebarPlayer::GetTimebarPlayer()->CreateSideTrack(UTimebarPlayer::GetTimebarPlayer()->SelectedTrack, InHeaderWidget, InContentWidget, InName);
	}
	else
	{
		UTimebarPlayer::GetTimebarPlayer()->CreateTrack(InHeaderWidget, InContentWidget, InName);
	}
}

void UTimebarController::DeleteTrack(UUserWidget* InTrackWidget)
{
	UTimebarPlayer::GetTimebarPlayer()->DeleteTrack(Cast<UTrackWidget>(InTrackWidget)->TargetTrack);
}

void UTimebarController::CreateClip(UUserWidget* InTrackWidget, float InStartTime, float InEndTime, FString InName)
{
	UTimebarPlayer::GetTimebarPlayer()->CreateClip(Cast<UTrackWidget>(InTrackWidget)->TargetTrack, InStartTime, InEndTime, InName);
	
}

void UTimebarController::DeleteClip(UUserWidget* InClipWidget)
{
	
	UTimebarPlayer::GetTimebarPlayer()->DeleteClip(Cast<UClipWidget>(InClipWidget)->TargetClip);
}

void UTimebarController::Start()
{
	UTimebarPlayer::GetTimebarPlayer()->Start();
}

void UTimebarController::Pause()
{
	UTimebarPlayer::GetTimebarPlayer()->Pause();
}

void UTimebarController::Stop()
{
	UTimebarPlayer::GetTimebarPlayer()->Stop();
}

void UTimebarController::SetCurrentTime(float InCurrentTime)
{
	UTimebarPlayer::GetTimebarPlayer()->CurrentTime = InCurrentTime;
	UTimebarPlayer::GetTimebarPlayer()->OnCurrentTimeChanged.Broadcast(InCurrentTime);
}




//// Manager
UTimebarManager* UTimebarController::GetTimebarManager(FName InManagerName)
{
	UTimebarManager* CurrentManager = nullptr;

	if (!ManagerMap.Contains(InManagerName)) // create
	{
		CurrentManager = NewObject<UTimebarManager>();
		CurrentManager->AddToRoot();
		ManagerMap.Add(InManagerName);
		ManagerMap[InManagerName].Manager = CurrentManager;
	}
	else // get
	{
		uint32 KeyHash = GetTypeHash(InManagerName);
		CurrentManager = ManagerMap.FindByHash(KeyHash, InManagerName)->Manager;
		if (!IsValid(CurrentManager))
		{
			UE_LOG(LogTemp, Warning, TEXT("No Manager in Map at UDragDropController::GetDragDropManager"));
			return nullptr;
		}
	}

	return CurrentManager;
}

void UTimebarController::SetTimebarManager(FName InManagerName)
{
	if (IsValid(UTimebarPlayer::GetTimebarPlayer()->CurrentManager))
	{
		UTimebarPlayer::GetTimebarPlayer()->CurrentManager->SetTracksVisibility(false);
	}

	UTimebarPlayer::GetTimebarPlayer()->CurrentManager = GetTimebarManager(InManagerName);
	UTimebarPlayer::GetTimebarPlayer()->CurrentManager->SetTracksVisibility(true);
}
