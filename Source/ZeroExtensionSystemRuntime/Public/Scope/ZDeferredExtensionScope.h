// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

class UZExtenderBaseInterface;
class IZExtensionScope;

namespace ZES
{
	struct ZEROEXTENSIONSYSTEMRUNTIME_API FZDeferredExtensionScope final : FNoncopyable
	{
		
		~FZDeferredExtensionScope();
		
		void RegisterExtender(UZExtenderBaseInterface* extender);
		void UnregisterExtender(UZExtenderBaseInterface* extender);
		
		void RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag);
		void UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag);
		
		void Open(IZExtensionScope* innerScope);
		void Open(TScriptInterface<IZExtensionScope> innerScope) { Open(innerScope.GetInterface()); }
		void Close();

	private:
		void InternalRegisterExtender(UZExtenderBaseInterface* extender);
		void InternalUnregisterExtender(UZExtenderBaseInterface* extender);
		
		void InternalRegisterExtendee(UObject* extendee, FGameplayTag channel);
		void InternalUnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel);

	private:
		enum class EZState
		{
			Unopened,
			Opened,
			Closed,
		};

		struct FZDeferredExtenderRegistration
		{
			TStrongObjectPtr<UZExtenderBaseInterface> Extender;
			friend bool operator==(const FZDeferredExtenderRegistration& lhs, const FZDeferredExtenderRegistration& rhs)
			{
				return lhs.Extender == rhs.Extender;
			}
		};

		struct FZDeferredExtendeeRegistration
		{
			FWeakObjectPtr Extendee;
			FGameplayTag Channel;
			friend bool operator==(const FZDeferredExtendeeRegistration& lhs, const FZDeferredExtendeeRegistration& rhs)
			{
				return lhs.Extendee == rhs.Extendee && lhs.Channel == rhs.Channel;
			}
		};

		EZState State = EZState::Unopened;
		TStrongObjectPtr<UObject> InnerScope;

		TArray<FZDeferredExtenderRegistration> DeferredExtenders;
		TArray<FZDeferredExtendeeRegistration> DeferredExtendees;
		
	};
}


