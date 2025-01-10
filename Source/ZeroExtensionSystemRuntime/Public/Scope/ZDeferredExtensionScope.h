// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

class UZExtenderBase;
class IZExtensionScope;

namespace ZES
{
	struct ZEROEXTENSIONSYSTEMRUNTIME_API FZDeferredExtensionScope final : FNoncopyable
	{
		
		~FZDeferredExtensionScope();
		
		void RegisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag);
		void UnregisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag);
		
		void RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag);
		void UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag);
		
		void Open(IZExtensionScope* innerScope);
		void Open(TScriptInterface<IZExtensionScope> innerScope) { Open(innerScope.GetInterface()); }
		void Close();

	private:
		void InternalRegisterExtender(UZExtenderBase* extender, FGameplayTag channel);
		void InternalUnregisterExtender(UZExtenderBase* extender, FGameplayTag channel);
		
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
			TStrongObjectPtr<UZExtenderBase> Extender;
			FGameplayTag Channel;
			friend bool operator==(const FZDeferredExtenderRegistration& lhs, const FZDeferredExtenderRegistration& rhs)
			{
				return lhs.Extender == rhs.Extender && lhs.Channel == rhs.Channel;
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


