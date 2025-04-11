#pragma once

#include "Modules/ModuleManager.h"
#include "../Private/TimebarDependency.h"
#include "../Private/TimebarController.h"


DECLARE_LOG_CATEGORY_EXTERN(Timebar, All, All);

class FTimebar : public IModuleInterface
{
public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;

	UTimebarController* Controller;
};