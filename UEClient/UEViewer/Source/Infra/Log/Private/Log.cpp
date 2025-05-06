#include "Log.h"

DEFINE_LOG_CATEGORY(Log);

#define LOCTEXT_NAMESPACE "FLog"

void FLog::StartupModule()
{
	UE_LOG(Log, Warning, TEXT("Log module has been loaded"));


}

void FLog::ShutdownModule()
{
	UE_LOG(Log, Warning, TEXT("Log module has been unloaded"));

	ULogManager::DestroyLogManager();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FLog, Log)