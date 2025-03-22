// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TimebarPlayer.h"
#include "PlayerDependency.h"
#include "../DragDrop/TimebarDragDropDependency.h"


// construct
UTimebarPlayer* UTimebarPlayer::Instance = nullptr;

UTimebarPlayer::UTimebarPlayer()
{
}

UTimebarPlayer::~UTimebarPlayer()
{
	if (IsValid(Instance) && Instance->IsRooted())
	{
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

		Instance->Shutdown();
		Instance->RemoveFromRoot();
	}

	
}

UTimebarPlayer* UTimebarPlayer::GetTimebarPlayer()
{
	if (!Instance)
	{
		Instance = NewObject<UTimebarPlayer>();
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





void UTimebarPlayer::Shutdown()
{
	if (TrackArray.Num() > 0)
	{
		for (auto& Track : TrackArray)
		{
			if (IsValid(Track) && Track->IsRooted())
			{
				Track->RemoveFromRoot();
				
				Track->MarkAsGarbage();
				Track = nullptr;
			}
		}

		TrackArray.Empty();
	}

	if (ClipArray.Num() > 0)
	{
		for (auto& Clip : ClipArray)
		{
			if (IsValid(Clip) && Clip->IsRooted())
			{
				Clip->RemoveFromRoot();
				Clip->MarkAsGarbage();
				Clip = nullptr;
			}
		}
		ClipArray.Empty();
	}


}



// Track
UTrack* UTimebarPlayer::CreateTrack(TSubclassOf<UUserWidget> InHeaderWidget, TSubclassOf<UUserWidget> InContentWidget, FString InName)
{
	// Create Object
	UTrack* NewTrack = NewObject<UTrack>(GWorld);
	NewTrack->AddToRoot();
	NewTrack->Name = InName;
	TrackArray.Add(NewTrack);

	// Create Widget
	UWorld* TargetWorld = NewTrack->GetWorld();
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
	// Create Object
	UTrack* NewTrack = NewObject<UTrack>(GWorld);
	NewTrack->AddToRoot();
	NewTrack->Name = InName;
	TrackArray.Add(NewTrack);

	// Create Widget
	UWorld* TargetWorld = NewTrack->GetWorld();
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
		TrackArray.Remove(InTrack);
		InTrack->RemoveFromRoot();
		InTrack->MarkAsGarbage();
		InTrack = nullptr;
	}
}





// Clip
UClip* UTimebarPlayer::CreateClip(UTrack* InTrack, float InStartTime, float InEndTime, FString InName)
{
	UClip* NewClip = NewObject<UClip>();
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
	if (IsValid(InClip))
	{
		ClipArray.Add(InClip);
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
	if (InClip)
	{
		if (SelectedClip == InClip)
		{
			SelectedClip = nullptr;
		}

		ClipArray.Remove(InClip);

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

		// CurrentTime에 맞춰 클립의 재생 여부를 확인하고 클립 상태 관리
		if (!ClipArray.IsEmpty())
		{
			for (auto& Clip : ClipArray)
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

		
	}
}

void UTimebarPlayer::SetCurrentTime(float InCurrentTime)
{
	CurrentTime = InCurrentTime;
	OnCurrentTimeChanged.Broadcast(CurrentTime);

	if (!ClipArray.IsEmpty())
	{
		for (auto& Clip : ClipArray)
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
}
