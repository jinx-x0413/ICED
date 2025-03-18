#include "HTTP_Connection.h"

DEFINE_LOG_CATEGORY(HTTP_Connection);

#define LOCTEXT_NAMESPACE "FHTTP_Connection"

void FHTTP_Connection::StartupModule()
{
	UE_LOG(HTTP_Connection, Warning, TEXT("HTTP_Connection module has been loaded"))
	HttpRequest = NewObject<UHttpRequest>();
	HttpRequest->AddToRoot();

	if (HttpRequest)
	{
		UE_LOG(LogTemp, Warning, TEXT("HttpRequest initialized successfully using NewObject"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to initialize HttpRequest!"));
	}
}

void FHTTP_Connection::ShutdownModule()
{
	UE_LOG(HTTP_Connection, Warning, TEXT("HTTP_Connection module has been unloaded"));
	if (IsValid(HttpRequest))
	{
		HttpRequest->RemoveFromRoot();
		HttpRequest->MarkAsGarbage();
		HttpRequest = nullptr;

	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHTTP_Connection, HTTP_Connection)