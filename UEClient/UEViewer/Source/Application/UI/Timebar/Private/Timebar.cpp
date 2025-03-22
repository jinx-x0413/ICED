#include "Timebar.h"

DEFINE_LOG_CATEGORY(Timebar);

#define LOCTEXT_NAMESPACE "FTimebar"

void FTimebar::StartupModule()
{
	UE_LOG(Timebar, Warning, TEXT("Timebar module has been loaded"));
	Controller = NewObject<UTimebarController>();
	Controller->AddToRoot();
}

void FTimebar::ShutdownModule()
{
	UE_LOG(Timebar, Warning, TEXT("Timebar module has been unloaded"));
	if (IsValid(Controller) && Controller->IsRooted())
	{
		Controller->RemoveFromRoot();
		Controller->MarkAsGarbage();
		Controller = nullptr;
	}

	if (IsValid(UTimebarPlayer::GetTimebarPlayer()))
	{
		UTimebarPlayer::GetTimebarPlayer()->RemoveFromRoot();
		UTimebarPlayer::GetTimebarPlayer()->MarkAsGarbage();
		
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FTimebar, Timebar)