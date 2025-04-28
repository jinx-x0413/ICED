// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimebarPlayer.h"
#include "PlayerDependency.h"
#include "../DragDrop/TimebarDragDropDependency.h"


// construct
UTimebarPlayer* UTimebarPlayer::Instance = nullptr;

UTimebarPlayer::UTimebarPlayer()
	: CurrentTime(0.0f)
	, SelectedTrack(nullptr)
	, SelectedClip(nullptr)
	/*, TrackArray()
	, ReverseTrackArray()
	, ClipArray()
	, ReverseClipArray()*/
{
}

void UTimebarPlayer::BeginDestroy()
{
	// Track, Clip 등 각종 데이터를 정리
	Shutdown();

	// 객체가 루트에 추가되었는지 체크
	if (this == Instance)
	{
		// 이벤트 바인딩 해제
		if (Instance->OnClipCreated.IsAlreadyBound(Instance, &UTimebarPlayer::AddClipToArray))
		{
			Instance->OnClipCreated.RemoveDynamic(Instance, &UTimebarPlayer::AddClipToArray);
		}

		if (Instance->OnTrackSelected.IsAlreadyBound(Instance, &UTimebarPlayer::SelectTrack))
		{
			Instance->OnTrackSelected.RemoveDynamic(Instance, &UTimebarPlayer::SelectTrack);
		}

		if (Instance->OnClipSelected.IsAlreadyBound(Instance, &UTimebarPlayer::SelectClip))
		{
			Instance->OnClipSelected.RemoveDynamic(Instance, &UTimebarPlayer::SelectClip);
		}

		if (Instance->OnFinished.IsAlreadyBound(Instance, &UTimebarPlayer::Stop))
		{
			Instance->OnFinished.RemoveDynamic(Instance, &UTimebarPlayer::Stop);
		}
	}

	// 객체 종료
	Shutdown();
	RemoveFromRoot();  // 루트에서 제거
	MarkAsGarbage();

	Super::BeginDestroy();  // 부모 클래스의 BeginDestroy 호출
}

UTimebarPlayer::~UTimebarPlayer()
{
}

void UTimebarPlayer::Shutdown()
{
	if (!Instance)
	{
		return;
	}

	if (IsValid(CurrentManager) && CurrentManager->IsRooted())
	{
		CurrentManager->RemoveFromRoot();
		CurrentManager->MarkAsGarbage();
		CurrentManager = nullptr;
	}
	


		// Track과 Clip 관련 배열들을 안전하게 처리
		/*if (Instance->TrackArray.Num() > 0)
		{
			for (auto& Track : Instance->TrackArray)
			{
				if (IsValid(Track) && Track->IsRooted())
				{
					Track->RemoveFromRoot();
					Track->MarkAsGarbage();
					Track = nullptr;
				}
			}
			Instance->TrackArray.Empty();
		}

		if (Instance->ReverseTrackArray.Num() > 0)
		{
			for (auto& Track : Instance->ReverseTrackArray)
			{
				if (IsValid(Track) && Track->IsRooted())
				{
					Track->RemoveFromRoot();
					Track->MarkAsGarbage();
					Track = nullptr;
				}
			}
			Instance->ReverseTrackArray.Empty();
		}

		if (Instance->ClipArray.Num() > 0)
		{
			for (auto& Clip : Instance->ClipArray)
			{
				if (IsValid(Clip) && Clip->IsRooted())
				{
					Clip->RemoveFromRoot();
					Clip->MarkAsGarbage();
					Clip = nullptr;
				}
			}
			Instance->ClipArray.Empty();
		}

		if (Instance->ReverseClipArray.Num() > 0)
		{
			for (auto& Clip : Instance->ReverseClipArray)
			{
				if (IsValid(Clip) && Clip->IsRooted())
				{
					Clip->RemoveFromRoot();
					Clip->MarkAsGarbage();
					Clip = nullptr;
				}
			}
			Instance->ReverseClipArray.Empty();
		}*/
}


UTimebarPlayer* UTimebarPlayer::GetTimebarPlayer()
{
	if (!Instance)
	{
		Instance = NewObject<UTimebarPlayer>(GetTransientPackage());
		Instance->AddToRoot();
		if (!Instance->OnClipCreated.IsAlreadyBound(Instance, &UTimebarPlayer::AddClipToArray))
		{
			Instance->OnClipCreated.AddDynamic(Instance, &UTimebarPlayer::AddClipToArray);
		}

		if (!Instance->OnTrackSelected.IsAlreadyBound(Instance, &UTimebarPlayer::SelectTrack))
		{
			Instance->OnTrackSelected.AddDynamic(Instance, &UTimebarPlayer::SelectTrack);
		}

		if (!Instance->OnClipSelected.IsAlreadyBound(Instance, &UTimebarPlayer::SelectClip))
		{
			Instance->OnClipSelected.AddDynamic(Instance, &UTimebarPlayer::SelectClip);
		}

		if (!Instance->OnFinished.IsAlreadyBound(Instance, &UTimebarPlayer::Stop))
		{
			Instance->OnFinished.AddDynamic(Instance, &UTimebarPlayer::Stop);
		}
	}
	return Instance;
}






// Track
UTrack* UTimebarPlayer::CreateTrack(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName)
{
	if (!IsValid(CurrentManager))
	{
		return nullptr;
	}


	// Create Object
	UTrack* NewTrack = NewObject<UTrack>(GetTransientPackage());
	NewTrack->AddToRoot();
	NewTrack->Name = InName;
	CurrentManager->TrackArray.Add(NewTrack);

	// Create Widget
	//UWorld* TargetWorld = NewTrack->GetWorld();
	UWorld* TargetWorld = GWorld;
	NewTrack->HeaderWidget = CreateWidget<UUserWidget>(TargetWorld, InHeaderWidget);
	NewTrack->ContentWidget = CreateWidget<UTrackWidget>(TargetWorld, InContentWidget);
	NewTrack->ContentWidget->TargetTrack = NewTrack;
	//NewTrack->ContentWidget->ExecCreateTrack(NewTrack->HeaderWidget, NewTrack->ContentWidget);

	OnTrackCreated.Broadcast(NewTrack, NewTrack->HeaderWidget, NewTrack->ContentWidget);
	//OnTrackSelected.Broadcast(NewTrack);

	return NewTrack;
}

UTrack* UTimebarPlayer::CreateSideTrack(UTrack* InParentTrack, TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName)
{
	if (!IsValid(CurrentManager))
	{
		return nullptr;
	}

	// Create Object
	UTrack* NewTrack = NewObject<UTrack>(GetTransientPackage());
	NewTrack->AddToRoot();
	NewTrack->Name = InName;
	CurrentManager->TrackArray.Add(NewTrack);

	// Create Widget
	//UWorld* TargetWorld = NewTrack->GetWorld();
	UWorld* TargetWorld = GWorld;
	NewTrack->HeaderWidget = CreateWidget<UUserWidget>(TargetWorld, InHeaderWidget);
	NewTrack->ContentWidget = CreateWidget<USideTrackWidget>(TargetWorld, InContentWidget);
	NewTrack->ContentWidget->TargetTrack = NewTrack;

	// Set Parent Track (Main Track)
	NewTrack->ParentTrack = InParentTrack;
	InParentTrack->SideTracks.Add(NewTrack);
	Cast<USideTrackWidget>(NewTrack->ContentWidget)->ParentTrack = InParentTrack;
	Cast<USideTrackWidget>(NewTrack->ContentWidget)->ParentTrackWidget = InParentTrack->ContentWidget;

	//NewTrack->ContentWidget->ExecCreateSideTrack(InParentTrack, NewTrack->HeaderWidget, NewTrack->ContentWidget);

	OnSideTrackCreated.Broadcast(InParentTrack, NewTrack, NewTrack->HeaderWidget, Cast<USideTrackWidget>(NewTrack->ContentWidget));
	OnTrackSelected.Broadcast(NewTrack);

	return nullptr;
}

UTrack* UTimebarPlayer::CreateComponent(TSubclassOf<UUserWidget> InSceneCaptureWidget, FString InName, USkeletalMeshComponent* InTargetComponent)
{
	if (!IsValid(CurrentManager))
	{
		return nullptr;
	}

	// Create Object
	UTrack* NewTrack = NewObject<UTrack>(GetTransientPackage());
	NewTrack->AddToRoot();
	NewTrack->Name = InName;
	NewTrack->TargetComponent = InTargetComponent;
	CurrentManager->TrackArray.Add(NewTrack);

	// Create Widget
	//UWorld* TargetWorld = NewTrack->GetWorld();
	UWorld* TargetWorld = GWorld;
	NewTrack->ComponentWidget = CreateWidget<USceneCaptureIcon>(TargetWorld, InSceneCaptureWidget);
	NewTrack->ComponentWidget->TargetTrack = NewTrack;
	//NewTrack->ContentWidget->ExecCreateTrack(NewTrack->HeaderWidget, NewTrack->ContentWidget);

	OnComponentTrackCreated.Broadcast(NewTrack, NewTrack->ComponentWidget);
	return NewTrack;
}
//
//UTrack* UTimebarPlayer::CreateComponentBackward(TSubclassOf<UUserWidget> InSceneCaptureWidget, FString InName, USkeletalMeshComponent* InTargetComponent)
//{
//	// Create Object
//	UTrack* NewTrack = NewObject<UTrack>(GetTransientPackage());
//	NewTrack->AddToRoot();
//	NewTrack->Name = InName;
//	NewTrack->TargetComponent = InTargetComponent;
//	ReverseTrackArray.Add(NewTrack);
//
//	// Create Widget
//	//UWorld* TargetWorld = NewTrack->GetWorld();
//	UWorld* TargetWorld = GWorld;
//	NewTrack->ComponentWidget = CreateWidget<USceneCaptureIcon>(TargetWorld, InSceneCaptureWidget);
//	NewTrack->ComponentWidget->TargetTrack = NewTrack;
//
//	OnComponentTrackBackwardCreated.Broadcast(NewTrack, NewTrack->ComponentWidget);
//	return NewTrack;
//}

void UTimebarPlayer::SelectTrack(UTrack* InTrack)
{
	if (InTrack)
	{
		SelectedTrack = InTrack;
	}
	else
	{
		SelectedTrack = nullptr;
	}
}

void UTimebarPlayer::DeleteTrack(UTrack* InTrack)
{
	if (!IsValid(CurrentManager))
	{
		return;
	}

	if (IsValid(InTrack))
	{
		// Clear Widget
		if (InTrack->HeaderWidget)
		{
			InTrack->HeaderWidget->RemoveFromParent();
		}

		if (InTrack->ContentWidget)
		{
			InTrack->ContentWidget->RemoveFromParent();
		}




		// Clear Track References
		if (InTrack->ParentTrack)
		{
			InTrack->ParentTrack->SideTracks.Remove(InTrack);
		}

		if (!InTrack->SideTracks.IsEmpty())
		{
			for (auto& SideTrack : InTrack->SideTracks)
			{
				DeleteTrack(SideTrack);
			}
			InTrack->SideTracks.Empty();
		}

		// Clear Track
		CurrentManager->TrackArray.Remove(InTrack);
		InTrack->RemoveFromRoot();
		InTrack->MarkAsGarbage();
		InTrack = nullptr;
	}
}





// Clip
UClip* UTimebarPlayer::CreateClip(UTrack* InTrack, float InStartTime, float InEndTime, FString InName)
{
	UClip* NewClip = NewObject<UClip>(GetTransientPackage());
	NewClip->TargetTrack = InTrack;
	NewClip->StartTime = InStartTime;
	NewClip->EndTime = InEndTime;
	NewClip->ClipLength = InEndTime - InStartTime;
	NewClip->Name = InName;
	NewClip->AddToRoot();

	OnClipCreated.Broadcast(InTrack, NewClip);
	OnTrackSelected.Broadcast(InTrack);
	return NewClip;
}

void UTimebarPlayer::AddClipToArray(UTrack* InTrack, UClip* InClip)
{
	if (!IsValid(CurrentManager))
	{
		return;
	}

	if (IsValid(InClip))
	{
		CurrentManager->ClipArray.Add(InClip);
	}

}

void UTimebarPlayer::SelectClip(UClip* InClip)
{
	if (InClip)
	{
		SelectedClip = InClip;
	}
	else
	{
		SelectedClip = nullptr;
	}
}

void UTimebarPlayer::DeleteClip(UClip* InClip)
{
	if (!IsValid(CurrentManager))
	{
		return;
	}

	if (InClip)
	{
		if (SelectedClip == InClip)
		{
			SelectedClip = nullptr;
		}

		CurrentManager->ClipArray.Remove(InClip);

		OnClipDeleted.Broadcast(InClip);
	}
}



// Timer
void UTimebarPlayer::Start()
{
	OnTrackSelected.Broadcast(nullptr);
	OnClipSelected.Broadcast(nullptr);

	if (GWorld)
	{
		switch (State)
		{
		case ETimebarState::Stopped:
			GWorld->GetTimerManager().SetTimer(MainTimer, this, &UTimebarPlayer::Run, 0.01f, true);
			State = ETimebarState::Running;
			break;
		case ETimebarState::Paused:
			if (GWorld->GetTimerManager().IsTimerPaused(MainTimer))
			{
				GWorld->GetTimerManager().UnPauseTimer(MainTimer);
			}
			else
			{
				GWorld->GetTimerManager().SetTimer(MainTimer, this, &UTimebarPlayer::Run, 0.01f, true);
			}

			State = ETimebarState::Running;
			break;
		case ETimebarState::Running:
			break;
		default:
			GWorld->GetTimerManager().SetTimer(MainTimer, this, &UTimebarPlayer::Run, 0.01f, true);
			State = ETimebarState::Running;
			break;
		}

		OnStarted.Broadcast();
		OnStateChanged.Broadcast(State);
	}
}

void UTimebarPlayer::Pause()
{
	if (GWorld && State == ETimebarState::Running)
	{
		GWorld->GetTimerManager().PauseTimer(MainTimer);
		State = ETimebarState::Paused;
		OnStateChanged.Broadcast(State);
	}
}

void UTimebarPlayer::Stop()
{
	if (GWorld && (State == ETimebarState::Running) || (State == ETimebarState::Paused))
	{
		GWorld->GetTimerManager().ClearTimer(MainTimer);
		CurrentTime = 0.0f;
		OnCurrentTimeChanged.Broadcast(CurrentTime);

		if (IsValid(CurrentManager) && !CurrentManager->ClipArray.IsEmpty())
		{
			for (auto& Clip : CurrentManager->ClipArray)
			{
				if (IsValid(Clip))
				{
					Clip->Stop();
				}
			}
		}

		/*if (bIsPlayingBackward)
		{
			if (!ReverseClipArray.IsEmpty())
			{
				for (auto& Clip : ReverseClipArray)
				{
					if (IsValid(Clip))
					{
						Clip->Stop();
					}
				}

			}
		}
		else
		{
			if (!ClipArray.IsEmpty())
			{
				for (auto& Clip : ClipArray)
				{
					if (IsValid(Clip))
					{
						Clip->Stop();
					}
				}

			}
		}*/

		State = ETimebarState::Stopped;
		OnStateChanged.Broadcast(State);
	}
}

void UTimebarPlayer::Run()
{
	// 게임의 실제 시간으로 CurrentTime 업데이트
	if (GWorld)
	{
		CurrentTime += GWorld->GetDeltaSeconds(); // DeltaTime을 사용하여 시간 진행
		OnCurrentTimeChanged.Broadcast(CurrentTime);

		if (IsValid(CurrentManager) && !CurrentManager->ClipArray.IsEmpty())
		{
			for (auto& Clip : CurrentManager->ClipArray)
			{
				if (IsValid(Clip) && Clip->ShouldPlay(CurrentTime))  // 클립이 재생될 시간인지 체크
				{
					Clip->Play(CurrentTime);  // 클립 재생
				}
				else if (IsValid(Clip))
				{
					Clip->Stop();  // 클립 일시정지
				}
			}
		}

		// CurrentTime에 맞춰 클립의 재생 여부를 확인하고 클립 상태 관리
		//if (!ClipArray.IsEmpty())
		//{
		//	for (auto& Clip : ClipArray)
		//	{
		//		if (IsValid(Clip) && Clip->ShouldPlay(CurrentTime))  // 클립이 재생될 시간인지 체크
		//		{
		//			Clip->Play(CurrentTime);  // 클립 재생
		//		}
		//		else if (IsValid(Clip))
		//		{
		//			Clip->Stop();  // 클립 일시정지
		//		}
		//	}
		//}

		//if (!TrackArray.IsEmpty())
		//{
		//	for (auto& Track : TrackArray)
		//	{
		//		if (IsValid(Track) && Track->IsPlaying())
		//		{
		//			Track->Play();
		//		}
		//		else if (IsValid(Track))
		//		{
		//			Track->Stop();
		//		}
		//	}
		//}


	}
}

void UTimebarPlayer::SetCurrentTime(float InCurrentTime)
{
	CurrentTime = InCurrentTime;
	OnCurrentTimeChanged.Broadcast(CurrentTime);

	if (IsValid(CurrentManager) && !CurrentManager->ClipArray.IsEmpty())
	{
		for (auto& Clip : CurrentManager->ClipArray)
		{
			if (IsValid(Clip) && Clip->ShouldPlay(CurrentTime))  // 클립이 재생될 시간인지 체크
			{
				Clip->Play(CurrentTime);  // 클립 재생
			}
			else if (IsValid(Clip))
			{
				Clip->Stop();  // 클립 일시정지
			}
		}
	}

	//if (bIsPlayingBackward)
	//{
	//	if (!ReverseClipArray.IsEmpty())
	//	{
	//		for (auto& Clip : ReverseClipArray)
	//		{
	//			if (IsValid(Clip) && Clip->ShouldPlay(CurrentTime))  // 클립이 재생될 시간인지 체크
	//			{
	//				Clip->Play(CurrentTime);  // 클립 재생
	//			}
	//			else if (IsValid(Clip))
	//			{
	//				Clip->Stop();  // 클립 일시정지
	//			}
	//		}
	//	}
	//}
	//else
	//{
	//	if (!ClipArray.IsEmpty())
	//	{
	//		for (auto& Clip : ClipArray)
	//		{
	//			if (IsValid(Clip) && Clip->ShouldPlay(CurrentTime))  // 클립이 재생될 시간인지 체크
	//			{
	//				Clip->Play(CurrentTime);  // 클립 재생
	//			}
	//			else if (IsValid(Clip))
	//			{
	//				Clip->Stop();  // 클립 일시정지
	//			}
	//		}
	//	}
	//}
}
//
//void UTimebarPlayer::RunBackward()
//{
//	// 게임의 실제 시간으로 CurrentTime 업데이트
//	if (GWorld)
//	{
//		CurrentTime += GWorld->GetDeltaSeconds(); // DeltaTime을 사용하여 시간 진행
//		OnCurrentTimeChanged.Broadcast(CurrentTime);
//
//		// CurrentTime에 맞춰 클립의 재생 여부를 확인하고 클립 상태 관리
//		if (!ReverseClipArray.IsEmpty())
//		{
//			for (auto& Clip : ReverseClipArray)
//			{
//				if (IsValid(Clip) && Clip->ShouldPlay(CurrentTime))  // 클립이 재생될 시간인지 체크
//				{
//					Clip->Play(CurrentTime);  // 클립 재생
//				}
//				else if (IsValid(Clip))
//				{
//					Clip->Stop();  // 클립 일시정지
//				}
//			}
//		}
//
//		if (!ReverseTrackArray.IsEmpty())
//		{
//			for (auto& Track : ReverseTrackArray)
//			{
//				if (IsValid(Track) && Track->IsPlaying())
//				{
//					Track->Play();
//				}
//				else if (IsValid(Track))
//				{
//					Track->Stop();
//				}
//			}
//		}
//
//
//	}
//}

void UTimebarPlayer::SetCurrentManager(UTimebarManager* InManager)
{
	CurrentManager = InManager;
	OnTimebarManagerSiwtched.Broadcast(InManager);
}
