// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZeroExtendeeFilterBase.generated.h"

UCLASS(Abstract, NotBlueprintable, DefaultToInstanced, EditInlineNew, Const)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZeroExtendeeFilterBase : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Passes(const UObject* extendee) const { PURE_VIRTUAL(UZeroExtendeeFilterBase::Passes); return false; }
	
};


