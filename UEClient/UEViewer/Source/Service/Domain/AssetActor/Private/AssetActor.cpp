#include "AssetActor.h"

DEFINE_LOG_CATEGORY(AssetActor);

#define LOCTEXT_NAMESPACE "FAssetActor"

void FAssetActor::StartupModule()
{
	UE_LOG(AssetActor, Warning, TEXT("AssetActor module has been loaded"));

	if (!Controller)
	{
		Controller = NewObject<UAssetActorController>();
		Controller->AddToRoot();
	}
}

void FAssetActor::ShutdownModule()
{
	UE_LOG(AssetActor, Warning, TEXT("AssetActor module has been unloaded"));

	if (IsValid(Controller) && Controller->IsRooted())
	{
		Controller->RemoveFromRoot();
		Controller->MarkAsGarbage();
		Controller = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAssetActor, AssetActor)