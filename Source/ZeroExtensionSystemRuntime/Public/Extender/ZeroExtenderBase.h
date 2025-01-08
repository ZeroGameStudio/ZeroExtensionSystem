// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "ZeroExtenderBase.generated.h"

UCLASS(MinimalAPI, Abstract, NotBlueprintable, DefaultToInstanced, EditInlineNew, Const)
class UZeroExtenderBase : public UObject
{
	GENERATED_BODY()

public:
	bool TryExtend(UObject* extendee);
	bool TryRevert(UObject* extendee, bool pendingKill);

	virtual FGameplayTag GetExtensionKey() const { PURE_VIRTUAL(UZeroExtenderBase::GetExtensionKey); return {}; }

protected:
	virtual bool Extend(UObject* extendee) { PURE_VIRTUAL(UZeroExtenderBase::Extend); return false; }
	virtual bool Revert(UObject* extendee, bool pendingKill) { PURE_VIRTUAL(UZeroExtenderBase::Revert); return false; }

	virtual bool CanExtend(UObject* extendee) const { PURE_VIRTUAL(UZeroExtenderBase::CanExtend); return false; }
	virtual bool IsTrivialRevert(UObject* extendee, bool pendingKill) const { return false; }
	virtual bool TreatsGarbageAsPendingKill(UObject* extendee) const { return true; }
	
};


