// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZGameExtensionScope.h"

void UZGameExtensionScope::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.RegisterExtender(extender);
}

void UZGameExtensionScope::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	ExtensionScope.UnregisterExtender(extender);
}

void UZGameExtensionScope::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	ExtensionScope.RegisterExtendee(extendee, channel);
}

void UZGameExtensionScope::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	ExtensionScope.UnregisterExtendee(extendee, destroying, channel);
}

void UZGameExtensionScope::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	ExtensionScope.Open(Create(this, "GameExtensionScope"));
}

void UZGameExtensionScope::Deinitialize()
{
	ExtensionScope.Close();
	
	Super::Deinitialize();
}


