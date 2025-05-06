#include "LogManager.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/Paths.h"

#include "LogDependency.h"

ULogManager* ULogManager::Instance = nullptr;
// construct
ULogManager::ULogManager()
{
}

ULogManager::~ULogManager()
{
 
}

ULogManager* ULogManager::GetLogManager()
{
    if (!Instance)
    {
        Instance = NewObject<ULogManager>();
        Instance->AddToRoot();
    }
    return Instance;
}

void ULogManager::DestroyLogManager()
{
    if (Instance)
    {
        Instance->RemoveFromRoot();
        Instance->MarkAsGarbage();
        Instance = nullptr;
    }
    
}



// feature
void ULogManager::ParseAuthToken()
{
    if (FParse::Value(FCommandLine::Get(), TEXT("-AuthToken="), AuthToken))
    {
        UE_LOG(LogTemp, Log, TEXT("[LoginManager] Token received: %s"), *AuthToken);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("[LoginManager] No Auth Token Found."));
    }
}

FString ULogManager::GetToken() const
{
    return AuthToken;
}

bool ULogManager::IsLoggedIn() const
{
    return !AuthToken.IsEmpty();
}
