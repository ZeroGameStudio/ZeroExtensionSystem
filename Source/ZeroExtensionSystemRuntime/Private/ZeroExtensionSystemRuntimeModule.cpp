// Copyright Zero Games. All Rights Reserved.

#include "ZeroExtensionSystemRuntimeModule.h"

class FZeroExtensionSystemRuntimeModule : public IZeroExtensionSystemRuntimeModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZeroExtensionSystemRuntimeModule, ZeroExtensionSystemRuntime)

void FZeroExtensionSystemRuntimeModule::StartupModule()
{
}

void FZeroExtensionSystemRuntimeModule::ShutdownModule()
{
}


