// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZDeferredExtensionScope.h"

#include "Extender/ZExtenderBaseInterface.h"
#include "Scope/ZExtensionScope.h"

ZES::FZDeferredExtensionScope::~FZDeferredExtensionScope()
{
	Close();
}

void ZES::FZDeferredExtensionScope::RegisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channel)
{
	if (State == EZState::Unopened)
	{
		DeferredExtenders.AddUnique({ TStrongObjectPtr<UZExtenderBaseInterface> { extender }, channel });
	}
	else if (InnerScope)
	{
		InternalRegisterExtender(extender, channel);
	}
}

void ZES::FZDeferredExtensionScope::UnregisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channel)
{
	if (State == EZState::Unopened)
	{
		DeferredExtenders.Remove({ TStrongObjectPtr<UZExtenderBaseInterface> { extender }, channel });
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

void ZES::FZDeferredExtensionScope::Open(IZExtensionScope* innerScope)
{
	if (State != EZState::Unopened)
	{
		return;
	}

	State = EZState::Opened;

	InnerScope = TStrongObjectPtr<UObject> { innerScope->_getUObject() };

	for (const auto& deferredExtender : DeferredExtenders)
	{
		if (UZExtenderBaseInterface* extender = deferredExtender.Extender.Get())
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

	if (IZExtensionScope* scope = Cast<IZExtensionScope>(InnerScope.Get()))
	{
		scope->TryClose();
	}

	InnerScope = nullptr;
}

void ZES::FZDeferredExtensionScope::InternalRegisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channel)
{
	IZExtensionScope* scope = CastChecked<IZExtensionScope>(InnerScope.Get());
	scope->ExtensionScope_RegisterExtender(extender, channel);
}

void ZES::FZDeferredExtensionScope::InternalUnregisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channel)
{
	IZExtensionScope* scope = CastChecked<IZExtensionScope>(InnerScope.Get());
	scope->ExtensionScope_UnregisterExtender(extender, channel);
}

void ZES::FZDeferredExtensionScope::InternalRegisterExtendee(UObject* extendee, FGameplayTag channel)
{
	IZExtensionScope* scope = CastChecked<IZExtensionScope>(InnerScope.Get());
	scope->ExtensionScope_RegisterExtendee(extendee, channel);
}

void ZES::FZDeferredExtensionScope::InternalUnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel)
{
	IZExtensionScope* scope = CastChecked<IZExtensionScope>(InnerScope.Get());
	scope->ExtensionScope_UnregisterExtendee(extendee, destroying, channel);
}


