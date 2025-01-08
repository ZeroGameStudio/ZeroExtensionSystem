// Copyright Zero Games. All Rights Reserved.

#pragma once

class IZeroExtensionSystemEditorModule : public IModuleInterface
{
public:
	static FORCEINLINE IZeroExtensionSystemEditorModule& Get()
	{
		static IZeroExtensionSystemEditorModule& GSingleton = FModuleManager::LoadModuleChecked<IZeroExtensionSystemEditorModule>("ZeroExtensionSystemEditor");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZeroExtensionSystemEditor");
	}
};


