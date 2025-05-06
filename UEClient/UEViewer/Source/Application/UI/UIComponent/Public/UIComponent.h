#pragma once

#include "Modules/ModuleManager.h"
#include "../Private/UIComponentDependency.h"

DECLARE_LOG_CATEGORY_EXTERN(UIComponent, All, All);

class UTableController;

class FUIComponent : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;


	// controller
public:
	UTableController* TableController;
};