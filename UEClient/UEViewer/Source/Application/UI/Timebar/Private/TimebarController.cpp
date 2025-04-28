#include "TimebarController.h"
#include "TimebarDependency.h"

// Construct
UTimebarController::UTimebarController()
{
}

void UTimebarController::BeginDestroy()
{
	Shutdown();

	RemoveFromRoot();
	MarkAsGarbage();

	Super::BeginDestroy();
}

UTimebarController::~UTimebarController()
{
}

void UTimebarController::Shutdown()
{
	if (!ManagerMap.IsEmpty())
	{
		ManagerMap.Empty();
	}
}




// Business Logic
void UTimebarController::CreateTrack(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName)
{
	UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer();
	if (!IsValid(Player)) return;

	bool bIsSideTrack = InContentWidget->IsChildOf(USideTrackWidget::StaticClass());
	if (bIsSideTrack && Player->SelectedTrack)
	{
		Player->CreateSideTrack(Player->SelectedTrack, InHeaderWidget, InContentWidget, InName);
	}
	else
	{
		Player->CreateTrack(InHeaderWidget, InContentWidget, InName);
	}
}

void UTimebarController::DeleteTrack(UUserWidget* InTrackWidget)
{
	UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer();
	if (!IsValid(Player)) return;

	if (UTrackWidget* TrackWidget = Cast<UTrackWidget>(InTrackWidget))
	{
		Player->DeleteTrack(TrackWidget->TargetTrack);
	}
}

void UTimebarController::CreateClip(UUserWidget* InTrackWidget, float InStartTime, float InEndTime, FString InName)
{
	UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer();
	if (!IsValid(Player)) return;

	if (UTrackWidget* TrackWidget = Cast<UTrackWidget>(InTrackWidget))
	{
		Player->CreateClip(TrackWidget->TargetTrack, InStartTime, InEndTime, InName);
	}
}

void UTimebarController::DeleteClip(UUserWidget* InClipWidget)
{
	UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer();
	if (!IsValid(Player)) return;

	if (UClipWidget* ClipWidget = Cast<UClipWidget>(InClipWidget))
	{
		Player->DeleteClip(ClipWidget->TargetClip);
	}
}

void UTimebarController::Start()
{
	if (UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer())
	{
		Player->Start();
	}
}

void UTimebarController::Pause()
{
	if (UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer())
	{
		Player->Pause();
	}
}

void UTimebarController::Stop()
{
	if (UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer())
	{
		Player->Stop();
	}
}

void UTimebarController::SetCurrentTime(float InCurrentTime)
{
	if (UTimebarPlayer* Player = UTimebarPlayer::GetTimebarPlayer())
	{
		Player->CurrentTime = InCurrentTime;
		Player->OnCurrentTimeChanged.Broadcast(InCurrentTime);
	}
}

UTimebarManager* UTimebarController::GetManager(FName InManagerName)
{
	UTimebarManager* CurrentManager = nullptr;


	if (ManagerMap.Contains(InManagerName)) // get
	{
		//uint32 Hash = GetTypeHash(InManagerName);
		CurrentManager = ManagerMap[InManagerName].Manager;
	}
	else // create
	{
		CurrentManager = NewObject<UTimebarManager>(GetTransientPackage());
		CurrentManager->AddToRoot();
		FTimebarManagerWrapper NewStruct;
		NewStruct.Manager = CurrentManager;
		ManagerMap.Add(InManagerName, NewStruct);
	}


	return CurrentManager;
}
