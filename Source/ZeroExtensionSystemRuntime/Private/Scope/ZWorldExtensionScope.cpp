// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZWorldExtensionScope.h"

#include "ZeroExtensionSystemRuntimeSettings.h"

void UZWorldExtensionScope::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void UZWorldExtensionScope::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void UZWorldExtensionScope::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void UZWorldExtensionScope::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

bool UZWorldExtensionScope::ShouldCreateSubsystem(UObject* Outer) const
{
	return GetDefault<UZeroExtensionSystemRuntimeSettings>()->IsWorldExtensionScopeEnabled();
}

void UZWorldExtensionScope::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	ExtensionScope.Open(Create(this, "GameExtensionScope"));
}

void UZWorldExtensionScope::Deinitialize()
{
	ExtensionScope.Close();
	
	Super::Deinitialize();
}


