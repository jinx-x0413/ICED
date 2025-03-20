#include "AssetContent.h"

DEFINE_LOG_CATEGORY(AssetContent);

#define LOCTEXT_NAMESPACE "FAssetContent"

void FAssetContent::StartupModule()
{
	UE_LOG(AssetContent, Warning, TEXT("AssetContent module has been loaded"));

	if (!Controller)
	{
		Controller = NewObject<UAssetContentController>();
		Controller->AddToRoot();
	}
}

void FAssetContent::ShutdownModule()
{
	UE_LOG(AssetContent, Warning, TEXT("AssetContent module has been unloaded"));

	if (IsValid(Controller) && Controller->IsRooted())
	{
		Controller->RemoveFromRoot();
		Controller->MarkAsGarbage();
		Controller = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAssetContent, AssetContent)