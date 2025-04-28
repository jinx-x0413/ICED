#include "Timebar.h"

DEFINE_LOG_CATEGORY(Timebar);

#define LOCTEXT_NAMESPACE "FTimebar"

void FTimebar::StartupModule()
{
	UE_LOG(Timebar, Warning, TEXT("Timebar module has been loaded"));

	Controller = NewObject<UTimebarController>();
	if (IsValid(Controller))
	{
		Controller->AddToRoot();
	}
}

void FTimebar::ShutdownModule()
{
	UE_LOG(Timebar, Warning, TEXT("Timebar module has been unloaded"));

	// Controller GC 처리
	if (IsValid(Controller) && Controller->IsRooted())
	{
		Controller->RemoveFromRoot();
		Controller->MarkAsGarbage();  // BeginDestroy 호출 유도
		Controller = nullptr;
	}

	// TimebarPlayer GC 처리
	UTimebarPlayer* PlayerInstance = UTimebarPlayer::GetTimebarPlayer();
	if (IsValid(PlayerInstance) && PlayerInstance->IsRooted())
	{
		PlayerInstance->RemoveFromRoot();
		PlayerInstance->MarkAsGarbage();  // BeginDestroy 호출 유도
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTimebar, Timebar)
