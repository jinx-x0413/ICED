#include "Command.h"

DEFINE_LOG_CATEGORY(Command);

#define LOCTEXT_NAMESPACE "FCommand"

void FCommand::StartupModule()
{
	UE_LOG(Command, Warning, TEXT("Command module has been loaded"));

	if (!Stack)
	{
		Stack = NewObject<UCommandStack>();
		Stack->AddToRoot();
	}
}

void FCommand::ShutdownModule()
{
	UE_LOG(Command, Warning, TEXT("Command module has been unloaded"));

	if (IsValid(Stack) && Stack->IsRooted())
	{
		Stack->RemoveFromRoot();
		Stack->MarkAsGarbage();
		Stack = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCommand, Command)