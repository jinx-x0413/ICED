#pragma once

#include "Modules/ModuleManager.h"
#include "Service/Domain/AssetContent/Private/AssetContentDependency.h"

DECLARE_LOG_CATEGORY_EXTERN(AssetContent, All, All);

class FAssetContent : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;



	// business logic
public:
	UAssetContentController* Controller;
};