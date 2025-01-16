// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZPlayerExtensionScope.h"

#include "ZeroExtensionSystemRuntimeSettings.h"

void UZPlayerExtensionScope::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void UZPlayerExtensionScope::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void UZPlayerExtensionScope::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void UZPlayerExtensionScope::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

bool UZPlayerExtensionScope::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetDefault<UZeroExtensionSystemRuntimeSettings>()->IsPlayerExtensionScopeEnabled();
}

void UZPlayerExtensionScope::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	ExtensionScope.Open(Create(this, "GameExtensionScope"));
}

void UZPlayerExtensionScope::Deinitialize()
{
	ExtensionScope.Close();
	
	Super::Deinitialize();
}


