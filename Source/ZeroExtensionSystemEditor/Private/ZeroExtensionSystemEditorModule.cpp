// Copyright Zero Games. All Rights Reserved.

#include "ZeroExtensionSystemEditorModule.h"

class FZeroExtensionSystemEditorModule : public IZeroExtensionSystemEditorModule
{
	// Begin IModuleInterface
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	// End IModuleInterface
};

IMPLEMENT_MODULE(FZeroExtensionSystemEditorModule, ZeroExtensionSystemEditor)

void FZeroExtensionSystemEditorModule::StartupModule()
{
}

void FZeroExtensionSystemEditorModule::ShutdownModule()
{
}


