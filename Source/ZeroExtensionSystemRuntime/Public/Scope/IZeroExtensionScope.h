// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

class UZeroExtenderBase;

namespace ZES
{
	class IZeroExtensionScope
	{
	public:
		static IZeroExtensionScope* Create(FName name);
	public:
		virtual void RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel) = 0;
		virtual void UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel) = 0;
		
		virtual void RegisterExtendee(UObject* extendee, FGameplayTag channel) = 0;
		virtual void UnregisterExtendee(UObject* extendee, FGameplayTag channel, bool pendingKill) = 0;
	public:
		virtual FName GetExtensionScopeName() const = 0;
	public:
		virtual ~IZeroExtensionScope(){}
	};
}


