#include "TimebarController.h"
#include "TimebarDependency.h"

// Construct
UTimebarController::UTimebarController()
{
}

void UTimebarController::BeginDestroy()
{
	Super::BeginDestroy();

	// 필요 시 여기에 정리 로직 삽입 (예: 이벤트 바인딩 해제 등)
	// 현재는 Controller가 생성한 객체를 직접 관리하지 않으므로 비워둠
}

UTimebarController::~UTimebarController()
{
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
