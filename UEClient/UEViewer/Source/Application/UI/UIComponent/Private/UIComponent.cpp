#include "UIComponent.h"

DEFINE_LOG_CATEGORY(UIComponent);

#define LOCTEXT_NAMESPACE "FUIComponent"

void FUIComponent::StartupModule()
{
	UE_LOG(UIComponent, Warning, TEXT("UIComponent module has been loaded"));
}

void FUIComponent::ShutdownModule()
{
	UE_LOG(UIComponent, Warning, TEXT("UIComponent module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUIComponent, UIComponent)