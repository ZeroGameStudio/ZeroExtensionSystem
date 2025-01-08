// Copyright Zero Games. All Rights Reserved.

#include "ZeroExtensionScope.h"

#include "ZeroExtensionSystemRuntimeLogChannels.h"
#include "Extender/ZeroExtenderBase.h"

ZES::FZeroExtensionScope::~FZeroExtensionScope()
{
	bDead = true;
	
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

void ZES::FZeroExtensionScope::RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	if (bDead)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[ZES::FZeroExtensionScope::RegisterExtender (%s)] Can't perform operation because the extension scope is pending close!"), *GetExtensionScopeName().ToString());
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
		return;
	}

	InternalRegisterExtender(extender, channel);
}

void ZES::FZeroExtensionScope::UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	if (bDead)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[ZES::FZeroExtensionScope::UnregisterExtender (%s)] Can't perform operation because the extension scope is pending close!"), *GetExtensionScopeName().ToString());
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
		return;
	}

	InternalUnregisterExtender(extender, channel);
}

void ZES::FZeroExtensionScope::RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	if (bDead)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[ZES::FZeroExtensionScope::RegisterExtendee (%s)] Can't perform operation because the extension scope is pending close!"), *GetExtensionScopeName().ToString());
		return;
	}
	
	if (!extendee)
	{
		return;
	}

	if (!channel.IsValid())
	{
		return;
	}

	InternalRegisterExtendee(extendee, channel);
}

void ZES::FZeroExtensionScope::UnregisterExtendee(UObject* extendee, FGameplayTag channel, bool pendingKill)
{
	if (bDead)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[ZES::FZeroExtensionScope::UnregisterExtendee (%s)] Can't perform operation because the extension scope is pending close!"), *GetExtensionScopeName().ToString());
		return;
	}
	
	if (!extendee)
	{
		return;
	}

	if (!channel.IsValid())
	{
		return;
	}

	InternalUnregisterExtendee(extendee, channel, pendingKill);
}

void ZES::FZeroExtensionScope::AddReferencedObjects(FReferenceCollector& collector)
{
	for (const auto& pair : Registry)
	{
		pair.Value->AddReferencedObjects(collector);
	}
}

FString ZES::FZeroExtensionScope::GetReferencerName() const
{
	return "ZES::FZeroExtensionScope";
}

void ZES::FZeroExtensionScope::InternalRegisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId)
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

void ZES::FZeroExtensionScope::InternalUnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId)
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

void ZES::FZeroExtensionScope::InternalRegisterExtendee(UObject* extendee, FGameplayTag channelId)
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

void ZES::FZeroExtensionScope::InternalUnregisterExtendee(UObject* extendee, FGameplayTag channelId, bool pendingKill)
{
	FZChannel& channel = GetChannel(channelId);
	if (!channel.RemoveExtendee(extendee))
	{
		return;
	}

	channel.ForeachExtender([extendee, pendingKill](UZeroExtenderBase* extender)
	{
		extender->TryRevert(extendee, pendingKill);
	});
}

ZES::FZeroExtensionScope::FZChannel& ZES::FZeroExtensionScope::GetChannel(FGameplayTag id)
{
	if (TUniquePtr<FZChannel>* channel = Registry.Find(id))
	{
		return **channel;
	}

	FZChannel* channel = new FZChannel { FName { FString::Printf(TEXT("%s::%s"), *Name.ToString(), *id.ToString()) } };
	Registry.Emplace(id, channel);
	return *channel;
}

bool ZES::FZeroExtensionScope::FZChannel::AddExtender(UZeroExtenderBase* extender)
{
	FGameplayTag extensionKey = extender->GetExtensionKey();
	if (!ExtenderLookup.Contains(extensionKey))
	{
		ExtenderLookup.Emplace(extensionKey, extender);
		ensure(Extenders.AddUnique(extender) == Extenders.Num() - 1);
	}

	UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[ZES::FZeroExtensionScope::FZChannel::AddExtender (%s)] Duplicated extension key [%s]"), *Name.ToString(), *extensionKey.ToString());
	return false;
}

bool ZES::FZeroExtensionScope::FZChannel::RemoveExtender(UZeroExtenderBase* extender)
{
	if (!ensure(!bExtendersLocked))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[ZES::FZeroExtensionScope::FZChannel::RemoveExtender (%s)] Can't remove extender [%s] while registry is locked!"), *Name.ToString(), *extender->GetPathName());
		return false;
	}

	FGameplayTag extensionKey = extender->GetExtensionKey();
	TWeakObjectPtr<UZeroExtenderBase>* weakExtender = ExtenderLookup.Find(extensionKey);
	if (!weakExtender)
	{
		return false;
	}

	UZeroExtenderBase* existingExtender = weakExtender->Get();
	if (existingExtender != extender)
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[ZES::FZeroExtensionScope::FZChannel::RemoveExtender (%s)] Duplicated extension key [%s]"), *Name.ToString(), *extensionKey.ToString());
		return false;
	}

	ExtenderLookup.Remove(extensionKey);
	ensure(Extenders.Remove(extender));

	return true;
}

bool ZES::FZeroExtensionScope::FZChannel::AddExtendee(UObject* extendee)
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

bool ZES::FZeroExtensionScope::FZChannel::RemoveExtendee(UObject* extendee)
{
	if (!ensure(!bExtendeesLocked))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Error, TEXT("[ZES::FZeroExtensionScope::FZChannel::RemoveExtendee (%s)] Can't remove extendee [%s] while registry is locked!"), *Name.ToString(), *extendee->GetPathName());
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

void ZES::FZeroExtensionScope::FZChannel::ForeachExtender(TFunctionRef<void(UZeroExtenderBase*)> action)
{
	{
		TGuardValue guardedLock { bExtendersLocked, true };
		for (const auto& extender : Extenders)
		{
			// This can happen when extender is marked as garbage externally, force cleared by GC without unregistration.
			if (!extender)
			{
				UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[ZES::FZeroExtensionScope::FZChannel::ForeachExtender (%s)] Some extender was garbage collected without unregistration!"), *Name.ToString());
				continue;
			}
			
			action(extender);
		}
	}
}

void ZES::FZeroExtensionScope::FZChannel::ForeachExtendee(TFunctionRef<void(UObject*)> action)
{
	{
		TGuardValue guardedLock { bExtendeesLocked, true };
		for (const auto& weakExtendee : Extendees)
		{
			UObject* extendee = weakExtendee.Get();
			if (!extendee)
			{
				UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("[ZES::FZeroExtensionScope::FZChannel::ForeachExtendee (%s)] Some extendee was garbage collected without unregistration!"), *Name.ToString());
				continue;
			}
			
			action(extendee);
		}
	}
}

void ZES::FZeroExtensionScope::FZChannel::AddReferencedObjects(FReferenceCollector& collector)
{
	collector.AddReferencedObjects(Extenders);
}


