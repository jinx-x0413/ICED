#include "Command.h"

DEFINE_LOG_CATEGORY(Command);

#define LOCTEXT_NAMESPACE "FCommand"

void FCommand::StartupModule()
{
    UE_LOG(Command, Warning, TEXT("Command module has been loaded"));

    if (!Stack)
    {
        Stack = NewObject<UCommandStack>(GetTransientPackage());
        Stack->AddToRoot();  // 루트에 추가
    }
}

void FCommand::ShutdownModule()
{
    UE_LOG(Command, Warning, TEXT("Command module has been unloaded"));

    if (IsValid(Stack))
    {
        Stack->RemoveFromRoot();  // 루트에서 제거
        Stack->ConditionalBeginDestroy();  // 객체 파괴 시작
        Stack = nullptr;  // 더 이상 참조하지 않도록 nullptr로 설정
    }
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FCommand, Command)
