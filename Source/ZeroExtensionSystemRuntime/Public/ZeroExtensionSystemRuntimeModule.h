// Copyright Zero Games. All Rights Reserved.

#pragma once

class IZeroExtensionSystemRuntimeModule : public IModuleInterface
{
public:
	static FORCEINLINE IZeroExtensionSystemRuntimeModule& Get()
	{
		static IZeroExtensionSystemRuntimeModule& GSingleton = FModuleManager::LoadModuleChecked<IZeroExtensionSystemRuntimeModule>("ZeroExtensionSystemRuntime");
		return GSingleton;
	}

	static FORCEINLINE bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("ZeroExtensionSystemRuntime");
	}
};


