// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZEngineExtensionScope.h"

#include "ZeroExtensionSystemRuntimeLogChannels.h"

void UZEngineExtensionScope::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void UZEngineExtensionScope::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void UZEngineExtensionScope::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void UZEngineExtensionScope::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

void UZEngineExtensionScope::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	ExtensionScope.Open(Create(this, "EngineExtensionScope"));
}

void UZEngineExtensionScope::Deinitialize()
{
	ExtensionScope.Close();
	
	Super::Deinitialize();
}


