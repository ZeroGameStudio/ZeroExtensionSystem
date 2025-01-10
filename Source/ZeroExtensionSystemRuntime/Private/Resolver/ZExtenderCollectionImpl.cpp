// Copyright Zero Games. All Rights Reserved.

#include "Resolver/ZExtenderCollectionImpl.h"

#include "GameplayTagContainer.h"
#include "ZeroExtensionSystemRuntimeLogChannels.h"
#include "Extender/ZExtenderBaseInterface.h"
#include "Scope/ZExtensionScope.h"

void UZExtenderCollectionImpl::Initialize(const TArray<UZExtenderBaseInterface*>& extenders)
{
	if (!ensure(!bInitialized))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtenderCollectionImpl::Initialize] Collection is already initialized!"));
		return;
	}

	bInitialized = true;

	TSet<FGameplayTag> existingKeys;
	for (const auto& extender : extenders)
	{
		if (!extender)
		{
			continue;
		}

		if (!ensure(extender->GetExtensionKey().IsValid()))
		{
			UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtenderCollectionImpl::Initialize] Extender [%s] has an invalid extension key!"), *extender->GetName());
			continue;
		}
		
		if (!ensure(!existingKeys.Contains(extender->GetExtensionKey())))
		{
			UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtenderCollectionImpl::Initialize] Extension key [%s] already exists!"), *extender->GetExtensionKey().ToString());
			continue;
		}

#if DO_CHECK
		if (!ensure(extender->HasAllFlags(RF_Transient)))
		{
			UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtenderCollectionImpl::Initialize] Extender [%s] is not transient!"), *extender->GetName());
		}
#endif

		Extenders.Emplace(extender);
	}
}

void UZExtenderCollectionImpl::Register(IZExtensionScope* scope)
{
	if (!scope)
	{
		return;
	}
	
	if (!ensure(bInitialized))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtenderCollectionImpl::Register] Collection is uninitialized!"));
		return;
	}

	if (!ensure(!bRegistered))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtenderCollectionImpl::Register] Collection is already registered to another scope!"));
		return;
	}

	bRegistered = true;
	OwnerScope = scope->_getUObject();

	for (const auto& extender : Extenders)
	{
		scope->ExtensionScope_RegisterExtender(extender);
	}
}

void UZExtenderCollectionImpl::Unregister()
{
	if (!ensure(bRegistered))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZExtenderCollectionImpl::Unregister] Collection is not registered to any scope!"));
		return;
	}

	if (!IsValid(OwnerScope.GetObject()))
	{
		return;
	}

	for (const auto& extender : Extenders)
	{
		OwnerScope->ExtensionScope_UnregisterExtender(extender);
	}

	OwnerScope = nullptr;
}



