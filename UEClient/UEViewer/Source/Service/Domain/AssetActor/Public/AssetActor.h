#pragma once

#include "Modules/ModuleManager.h"
#include "Service/Domain/AssetActor/Private/AssetActorDependency.h"

DECLARE_LOG_CATEGORY_EXTERN(AssetActor, All, All);

class FAssetActor : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;




	// BusinessLogic
public:
	UAssetActorController* Controller;
};