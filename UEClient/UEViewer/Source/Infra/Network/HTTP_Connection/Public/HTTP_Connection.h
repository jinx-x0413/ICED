#pragma once

#include "Modules/ModuleManager.h"
#include "Infra/Network/HTTP_Connection/Private/HttpDependency.h"
#include "Infra/Network/HTTP_Connection/Private/HttpRequest.h"



DECLARE_LOG_CATEGORY_EXTERN(HTTP_Connection, All, All);

class FHTTP_Connection : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

public:
	UHttpRequest* HttpRequest;
};