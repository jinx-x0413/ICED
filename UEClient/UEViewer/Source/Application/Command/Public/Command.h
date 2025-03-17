#pragma once

#include "Modules/ModuleManager.h"
#include "Application/Command/Private/CommandDependency.h"

DECLARE_LOG_CATEGORY_EXTERN(Command, All, All);

class FCommand : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;



	// Business Logic
public:
	UCommandStack* Stack;
};