// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZeroExtensionSubsystem.h"

#include "ZeroExtensionSystemRuntimeLogChannels.h"

void UZeroExtensionSubsystem::ZeroExtensionScope_RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	ExtensionScope.RegisterExtender(extender, channel);
}

void UZeroExtensionSubsystem::ZeroExtensionScope_UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtender(extender, channel);
}

void UZeroExtensionSubsystem::ZeroExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void UZeroExtensionSubsystem::ZeroExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

void UZeroExtensionSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	ExtensionScope.Open(Create(this, "EngineExtensionScope"));
}

void UZeroExtensionSubsystem::Deinitialize()
{
	ExtensionScope.Close();
	
	Super::Deinitialize();
}


