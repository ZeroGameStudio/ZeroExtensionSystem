// Copyright Zero Games. All Rights Reserved.

#include "ZExtensionScopeImpl.h"

#include "ZDefaultExtensionChannels.h"
#include "ZeroExtensionSystemRuntimeLogChannels.h"
#include "Extender/ZExtenderBaseInterface.h"

void UZExtensionScopeImpl::AddReferencedObjects(UObject* self, FReferenceCollector& collector)
{
	Super::AddReferencedObjects(self, collector);
	
	for (const auto& pair : static_cast<UZExtensionScopeImpl*>(self)->Registry)
	{
		pair.Value->AddReferencedObjects(collector);
	}
}

void UZExtensionScopeImpl::BeginDestroy()
{
	ExtensionScope_Close();
	
	UObject::BeginDestroy();
}

void UZExtensionScopeImpl::ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender)
{
	if (bClosed)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZeroExtensionScopeImpl::RegisterExtender (%s)] Can't perform operation because the extension scope is pending close!"), *GetName());
		return;
	}
	
	if (!extender)
	{
		return;
	}

	if (!extender->GetExtensionKey().IsValid())
	{
		return;
	}

	if (!ensure(!ZES::ZExtenderBaseInterface_Private::FZVisitor { extender }.IsRegistered()))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZExtensionScopeImpl::ExtensionScope_RegisterExtender (%s)] Can't register extender [%s] because it is already registered to another scope!"), *GetName(), *extender->GetPathName());
		return;
	}

	FGameplayTag key = extender->GetExtensionKey();
	if (RegisteredExtensionKeys.Contains(key))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZExtensionScopeImpl::ExtensionScope_RegisterExtender (%s)] Duplicated extension key [%s]"), *GetName(), *key.ToString());
		return;
	}

	FGameplayTag channel = extender->GetExtensionChannel();
	if (!channel.IsValid())
	{
		channel = ZES::TAG_ExtensionChannel_Default;
	}

	InternalRegisterExtender(extender, channel);
}

void UZExtensionScopeImpl::ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender)
{
	if (bClosed)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZeroExtensionScopeImpl::UnregisterExtender (%s)] Can't perform operation because the extension scope is pending close!"), *GetName());
		return;
	}
	
	if (!extender)
	{
		return;
	}
	
	if (!extender->GetExtensionKey().IsValid())
	{
		return;
	}

	if (!ensure(ZES::ZExtenderBaseInterface_Private::FZVisitor { extender }.GetOwnerScope() == this))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZExtensionScopeImpl::ExtensionScope_UnregisterExtender (%s)] Can't unregister extender [%s] because it isn't registered to this scope!"), *GetName(), *extender->GetPathName());
		return;
	}

	FGameplayTag channel = extender->GetExtensionChannel();
	if (!channel.IsValid())
	{
		channel = ZES::TAG_ExtensionChannel_Default;
	}

	InternalUnregisterExtender(extender, channel);
}

void UZExtensionScopeImpl::ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	if (bClosed)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZeroExtensionScopeImpl::RegisterExtendee (%s)] Can't perform operation because the extension scope is pending close!"), *GetName());
		return;
	}
	
	if (!extendee)
	{
		return;
	}

	if (!channel.IsValid())
	{
		channel = ZES::TAG_ExtensionChannel_Default;
	}

	InternalRegisterExtendee(extendee, channel);
}

void UZExtensionScopeImpl::ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	if (bClosed)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZeroExtensionScopeImpl::UnregisterExtendee (%s)] Can't perform operation because the extension scope is pending close!"), *GetName());
		return;
	}
	
	if (!extendee)
	{
		return;
	}

	if (!channel.IsValid())
	{
		channel = ZES::TAG_ExtensionChannel_Default;
	}

	InternalUnregisterExtendee(extendee, channel, destroying);
}

void UZExtensionScopeImpl::ExtensionScope_Close()
{
	if (bClosed)
	{
		return;
	}
	
	bClosed = true;
	
	for (auto& pair : Registry)
	{
		FZChannel& channel = *pair.Value;
		channel.ForeachExtender([&channel](UZExtenderBaseInterface* extender)
		{
			channel.ForeachExtendee([extender](UObject* extendee)
			{
				// Don't change the container because it will be no longer used and we don't need to deal with iterator lock.
				extender->TryRevert(extendee, false);
			});
		});
	}
}

void UZExtensionScopeImpl::InternalRegisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channelId)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.AddExtender(extender))
	{
		return;
	}
	
	RegisteredExtensionKeys.Emplace(extender->GetExtensionKey());

	ZES::ZExtenderBaseInterface_Private::FZVisitor { extender }.HandleRegister(this);

	channel.ForeachExtendee([extender](UObject* extendee)
	{
		extender->TryExtend(extendee);
	});
}

void UZExtensionScopeImpl::InternalUnregisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channelId)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.RemoveExtender(extender))
	{
		return;
	}

	ZES::ZExtenderBaseInterface_Private::FZVisitor { extender }.HandleUnregister();

	ensure(RegisteredExtensionKeys.Remove(extender->GetExtensionKey()));
	
	channel.ForeachExtendee([extender](UObject* extendee)
	{
		extender->TryRevert(extendee, false);
	});
}

void UZExtensionScopeImpl::InternalRegisterExtendee(UObject* extendee, FGameplayTag channelId)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.AddExtendee(extendee))
	{
		return;
	}

	channel.ForeachExtender([extendee](UZExtenderBaseInterface* extender)
	{
		extender->TryExtend(extendee);
	});
}

void UZExtensionScopeImpl::InternalUnregisterExtendee(UObject* extendee, FGameplayTag channelId, bool destroying)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.RemoveExtendee(extendee))
	{
		return;
	}

	channel.ForeachExtender([extendee, destroying](UZExtenderBaseInterface* extender)
	{
		extender->TryRevert(extendee, destroying);
	});
}

UZExtensionScopeImpl::FZChannel& UZExtensionScopeImpl::GetChannel(FGameplayTag id)
{
	if (TUniquePtr<FZChannel>* channel = Registry.Find(id))
	{
		return **channel;
	}

	FZChannel* channel = new FZChannel { FName { FString::Printf(TEXT("%s::%s"), *Name.ToString(), *id.ToString()) } };
	Registry.Emplace(id, channel);
	return *channel;
}

bool UZExtensionScopeImpl::FZChannel::AddExtender(UZExtenderBaseInterface* extender)
{
	if (ensure(!ExtenderLookup.Contains(extender)))
	{
		ExtenderLookup.Emplace(extender);
		ensure(!Extenders.Contains(extender));
		Extenders.Emplace(extender);
		return true;
	}

	UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionScopeImpl::FZChannel::AddExtender (%s)] Duplicated extender [%s]"), *Name.ToString(), *extender->GetName());
	return false;
}

bool UZExtensionScopeImpl::FZChannel::RemoveExtender(UZExtenderBaseInterface* extender)
{
	if (!ensure(!bExtendersLocked))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionScopeImpl::FZChannel::RemoveExtender (%s)] Can't remove extender [%s] while registry is locked!"), *Name.ToString(), *extender->GetPathName());
		return false;
	}

	if (!ensure(ExtenderLookup.Remove(extender)))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionScopeImpl::FZChannel::RemoveExtender (%s)] Extender [%s] does not exist!"), *Name.ToString(), *extender->GetName());
		return false;
	}

	ensure(Extenders.Remove(extender));

	return true;
}

bool UZExtensionScopeImpl::FZChannel::AddExtendee(UObject* extendee)
{
	bool alreadyExists;
	ExtendeeLookup.Emplace(extendee, &alreadyExists);
	if (!alreadyExists)
	{
		Extendees.Emplace(extendee);
		return true;
	}

	return false;
}

bool UZExtensionScopeImpl::FZChannel::RemoveExtendee(UObject* extendee)
{
	if (!ensure(!bExtendeesLocked))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionScopeImpl::FZChannel::RemoveExtendee (%s)] Can't remove extendee [%s] while registry is locked!"), *Name.ToString(), *extendee->GetPathName());
		return false;
	}
	
	if (!ExtendeeLookup.Contains(extendee))
	{
		return false;
	}

	ExtendeeLookup.Remove(extendee);
	ensure(Extendees.Remove(extendee));

	return true;
}

void UZExtensionScopeImpl::FZChannel::ForeachExtender(TFunctionRef<void(UZExtenderBaseInterface*)> action)
{
	TGuardValue guardedLock { bExtendersLocked, true };
	for (const auto& extender : Extenders)
	{
		// This can happen when extender is marked as garbage externally, force cleared by GC without unregistration.
		if (!extender)
		{
			UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZeroExtensionScopeImpl::FZChannel::ForeachExtender (%s)] Some extender was garbage collected without unregistration!"), *Name.ToString());
			continue;
		}
			
		action(extender);
	}
}

void UZExtensionScopeImpl::FZChannel::ForeachExtendee(TFunctionRef<void(UObject*)> action)
{
	TGuardValue guardedLock { bExtendeesLocked, true };
	for (const auto& weakExtendee : Extendees)
	{
		UObject* extendee = weakExtendee.Get();
		if (!extendee)
		{
			UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[UZeroExtensionScopeImpl::FZChannel::ForeachExtendee (%s)] Some extendee was garbage collected without unregistration!"), *Name.ToString());
			continue;
		}
			
		action(extendee);
	}
}

void UZExtensionScopeImpl::FZChannel::AddReferencedObjects(FReferenceCollector& collector)
{
	collector.AddReferencedObjects(Extenders);
}


