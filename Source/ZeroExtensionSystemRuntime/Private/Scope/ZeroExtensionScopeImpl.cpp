// Copyright Zero Games. All Rights Reserved.

#include "ZeroExtensionScopeImpl.h"

#include "ZeroExtensionSystemChannels.h"
#include "ZeroExtensionSystemRuntimeLogChannels.h"
#include "Extender/ZeroExtenderBase.h"

void UZeroExtensionScopeImpl::AddReferencedObjects(UObject* self, FReferenceCollector& collector)
{
	Super::AddReferencedObjects(self, collector);
	
	for (const auto& pair : static_cast<UZeroExtensionScopeImpl*>(self)->Registry)
	{
		pair.Value->AddReferencedObjects(collector);
	}
}

void UZeroExtensionScopeImpl::BeginDestroy()
{
	ZeroExtensionScope_Close();
	
	UObject::BeginDestroy();
}

void UZeroExtensionScopeImpl::ZeroExtensionScope_RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
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

	if (!channel.IsValid())
	{
		channel = TAG_ExtensionChannel_Default;
	}

	InternalRegisterExtender(extender, channel);
}

void UZeroExtensionScopeImpl::ZeroExtensionScope_UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
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

	if (!channel.IsValid())
	{
		channel = TAG_ExtensionChannel_Default;
	}

	InternalUnregisterExtender(extender, channel);
}

void UZeroExtensionScopeImpl::ZeroExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel)
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
		channel = TAG_ExtensionChannel_Default;
	}

	InternalRegisterExtendee(extendee, channel);
}

void UZeroExtensionScopeImpl::ZeroExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
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
		channel = TAG_ExtensionChannel_Default;
	}

	InternalUnregisterExtendee(extendee, channel, destroying);
}

void UZeroExtensionScopeImpl::ZeroExtensionScope_Close()
{
	if (bClosed)
	{
		return;
	}
	
	bClosed = true;
	
	for (auto& pair : Registry)
	{
		FZChannel& channel = *pair.Value;
		channel.ForeachExtender([&channel](UZeroExtenderBase* extender)
		{
			channel.ForeachExtendee([extender](UObject* extendee)
			{
				// Don't change the container because it will be no longer used and we don't need to deal with iterator lock.
				extender->TryRevert(extendee, false);
			});
		});
	}
}

void UZeroExtensionScopeImpl::InternalRegisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.AddExtender(extender))
	{
		return;
	}

	channel.ForeachExtendee([extender](UObject* extendee)
	{
		extender->TryExtend(extendee);
	});
}

void UZeroExtensionScopeImpl::InternalUnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.RemoveExtender(extender))
	{
		return;
	}

	channel.ForeachExtendee([extender](UObject* extendee)
	{
		extender->TryRevert(extendee, false);
	});
}

void UZeroExtensionScopeImpl::InternalRegisterExtendee(UObject* extendee, FGameplayTag channelId)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.AddExtendee(extendee))
	{
		return;
	}

	channel.ForeachExtender([extendee](UZeroExtenderBase* extender)
	{
		extender->TryExtend(extendee);
	});
}

void UZeroExtensionScopeImpl::InternalUnregisterExtendee(UObject* extendee, FGameplayTag channelId, bool destroying)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.RemoveExtendee(extendee))
	{
		return;
	}

	channel.ForeachExtender([extendee, destroying](UZeroExtenderBase* extender)
	{
		extender->TryRevert(extendee, destroying);
	});
}

UZeroExtensionScopeImpl::FZChannel& UZeroExtensionScopeImpl::GetChannel(FGameplayTag id)
{
	if (TUniquePtr<FZChannel>* channel = Registry.Find(id))
	{
		return **channel;
	}

	FZChannel* channel = new FZChannel { FName { FString::Printf(TEXT("%s::%s"), *Name.ToString(), *id.ToString()) } };
	Registry.Emplace(id, channel);
	return *channel;
}

bool UZeroExtensionScopeImpl::FZChannel::AddExtender(UZeroExtenderBase* extender)
{
	FGameplayTag extensionKey = extender->GetExtensionKey();
	if (ensure(!ExtenderLookup.Contains(extensionKey)))
	{
		ExtenderLookup.Emplace(extensionKey, extender);
		ensure(Extenders.AddUnique(extender) == Extenders.Num() - 1);
	}

	UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionScopeImpl::FZChannel::AddExtender (%s)] Duplicated extension key [%s]"), *Name.ToString(), *extensionKey.ToString());
	return false;
}

bool UZeroExtensionScopeImpl::FZChannel::RemoveExtender(UZeroExtenderBase* extender)
{
	if (!ensure(!bExtendersLocked))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionScopeImpl::FZChannel::RemoveExtender (%s)] Can't remove extender [%s] while registry is locked!"), *Name.ToString(), *extender->GetPathName());
		return false;
	}

	FGameplayTag extensionKey = extender->GetExtensionKey();
	TWeakObjectPtr<UZeroExtenderBase>* weakExtender = ExtenderLookup.Find(extensionKey);
	if (!weakExtender)
	{
		return false;
	}

	UZeroExtenderBase* existingExtender = weakExtender->Get();
	if (!ensure(existingExtender == extender))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[UZeroExtensionScopeImpl::FZChannel::RemoveExtender (%s)] Duplicated extension key [%s]"), *Name.ToString(), *extensionKey.ToString());
		return false;
	}

	ExtenderLookup.Remove(extensionKey);
	ensure(Extenders.Remove(extender));

	return true;
}

bool UZeroExtensionScopeImpl::FZChannel::AddExtendee(UObject* extendee)
{
	bool alreadyExists;
	ExtendeeLookup.Emplace(extendee, &alreadyExists);
	if (!alreadyExists)
	{
		ensure(Extendees.Emplace(extendee) == Extenders.Num() - 1);
		return true;
	}

	return false;
}

bool UZeroExtensionScopeImpl::FZChannel::RemoveExtendee(UObject* extendee)
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

void UZeroExtensionScopeImpl::FZChannel::ForeachExtender(TFunctionRef<void(UZeroExtenderBase*)> action)
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

void UZeroExtensionScopeImpl::FZChannel::ForeachExtendee(TFunctionRef<void(UObject*)> action)
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

void UZeroExtensionScopeImpl::FZChannel::AddReferencedObjects(FReferenceCollector& collector)
{
	collector.AddReferencedObjects(Extenders);
}


