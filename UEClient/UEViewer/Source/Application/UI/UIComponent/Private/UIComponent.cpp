#include "UIComponent.h"

DEFINE_LOG_CATEGORY(UIComponent);

#define LOCTEXT_NAMESPACE "FUIComponent"

void FUIComponent::StartupModule()
{
	UE_LOG(UIComponent, Warning, TEXT("UIComponent module has been loaded"));

	if (!IsValid(TableController))
	{
		TableController = NewObject<UTableController>(GetTransientPackage());
		TableController->AddToRoot();
	}
}

void FUIComponent::ShutdownModule()
{
	UE_LOG(UIComponent, Warning, TEXT("UIComponent module has been unloaded"));

	if (IsValid(TableController) && TableController->IsRooted())
	{
		TableController->RemoveFromRoot();
		TableController->MarkAsGarbage();
		TableController = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUIComponent, UIComponent)