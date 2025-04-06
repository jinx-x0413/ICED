#include "Profiling.h"

DEFINE_LOG_CATEGORY(Profiling);

#define LOCTEXT_NAMESPACE "FProfiling"

void FProfiling::StartupModule()
{
	UE_LOG(Profiling, Warning, TEXT("Profiling module has been loaded"));
}

void FProfiling::ShutdownModule()
{
	UE_LOG(Profiling, Warning, TEXT("Profiling module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FProfiling, Profiling)