// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZDeferredExtensionScope.h"

#include "Extender/ZeroExtenderBase.h"
#include "Scope/ZeroExtensionScope.h"

ZES::FZDeferredExtensionScope::~FZDeferredExtensionScope()
{
	Close();
}

void ZES::FZDeferredExtensionScope::RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	if (State == EZState::Unopened)
	{
		DeferredExtenders.AddUnique({ TStrongObjectPtr<UZeroExtenderBase> { extender }, channel });
	}
	else if (InnerScope)
	{
		InternalRegisterExtender(extender, channel);
	}
}

void ZES::FZDeferredExtensionScope::UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	if (State == EZState::Unopened)
	{
		DeferredExtenders.Remove({ TStrongObjectPtr<UZeroExtenderBase> { extender }, channel });
	}
	else if (InnerScope)
	{
		InternalUnregisterExtender(extender, channel);
	}
}

void ZES::FZDeferredExtensionScope::RegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	if (State == EZState::Unopened)
	{
		DeferredExtendees.AddUnique({ extendee, channel });
	}
	else if (InnerScope)
	{
		InternalRegisterExtendee(extendee, channel);
	}
}

void ZES::FZDeferredExtensionScope::UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	if (State == EZState::Unopened)
	{
		DeferredExtendees.Remove({ extendee, channel });
	}
	else if (InnerScope)
	{
		InternalUnregisterExtendee(extendee, destroying, channel);
	}
}

void ZES::FZDeferredExtensionScope::Open(IZeroExtensionScope* innerScope)
{
	if (State != EZState::Unopened)
	{
		return;
	}

	State = EZState::Opened;

	InnerScope = TStrongObjectPtr<UObject> { innerScope->_getUObject() };

	for (const auto& deferredExtender : DeferredExtenders)
	{
		if (UZeroExtenderBase* extender = deferredExtender.Extender.Get())
		{
			InternalRegisterExtender(extender, deferredExtender.Channel);
		}
	}
	DeferredExtenders.Empty();
	
	for (const auto& deferredExtendee : DeferredExtendees)
	{
		if (UObject* extendee = deferredExtendee.Extendee.Get())
		{
			InternalRegisterExtendee(extendee, deferredExtendee.Channel);
		}
	}
	DeferredExtendees.Empty();
}

void ZES::FZDeferredExtensionScope::Close()
{
	if (State != EZState::Opened)
	{
		return;
	}

	State = EZState::Closed;

	if (IZeroExtensionScope* scope = Cast<IZeroExtensionScope>(InnerScope.Get()))
	{
		scope->TryClose();
	}

	InnerScope = nullptr;
}

void ZES::FZDeferredExtensionScope::InternalRegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	IZeroExtensionScope* scope = CastChecked<IZeroExtensionScope>(InnerScope.Get());
	scope->ZeroExtensionScope_RegisterExtender(extender, channel);
}

void ZES::FZDeferredExtensionScope::InternalUnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel)
{
	IZeroExtensionScope* scope = CastChecked<IZeroExtensionScope>(InnerScope.Get());
	scope->ZeroExtensionScope_UnregisterExtender(extender, channel);
}

void ZES::FZDeferredExtensionScope::InternalRegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	IZeroExtensionScope* scope = CastChecked<IZeroExtensionScope>(InnerScope.Get());
	scope->ZeroExtensionScope_RegisterExtendee(extendee, channel);
}

void ZES::FZDeferredExtensionScope::InternalUnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	IZeroExtensionScope* scope = CastChecked<IZeroExtensionScope>(InnerScope.Get());
	scope->ZeroExtensionScope_UnregisterExtendee(extendee, destroying, channel);
}


