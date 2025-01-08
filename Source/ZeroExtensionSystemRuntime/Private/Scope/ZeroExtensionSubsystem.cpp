// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZeroExtensionSubsystem.h"

#include "ZeroExtensionSystemRuntimeLogChannels.h"

void UZeroExtensionSubsystem::RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	if (!ensure(InnerExtensionScope))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionSubsystem::RegisterExtender] Engine extension scope is not available!"));
		return;
	}

	InnerExtensionScope->RegisterExtender(extender, channel);
}

void UZeroExtensionSubsystem::UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	if (!ensure(InnerExtensionScope))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionSubsystem::UnregisterExtender] Engine extension scope is not available!"));
		return;
	}

	InnerExtensionScope->UnregisterExtender(extender, channel);
}

void UZeroExtensionSubsystem::RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	if (!ensure(InnerExtensionScope))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionSubsystem::RegisterExtendee] Engine extension scope is not available!"));
		return;
	}

	InnerExtensionScope->RegisterExtendee(extendee, channel);
}

void UZeroExtensionSubsystem::UnregisterExtendee(UObject* extendee, FGameplayTag channel, bool pendingKill)
{
	if (!ensure(InnerExtensionScope))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionSubsystem::UnregisterExtendee] Engine extension scope is not available!"));
		return;
	}

	InnerExtensionScope->UnregisterExtendee(extendee, channel, pendingKill);
}

void UZeroExtensionSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	
	InnerExtensionScope = TUniquePtr<IZeroExtensionScope> { Create(GetExtensionScopeName()) };
}

void UZeroExtensionSubsystem::Deinitialize()
{
	InnerExtensionScope = nullptr;
	
	Super::Deinitialize();
}


