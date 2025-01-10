// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtendeeFilterBase.generated.h"

UCLASS(Abstract, NotBlueprintable, DefaultToInstanced, EditInlineNew, Const)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtendeeFilterBase : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Passes(const UObject* extendee) const { PURE_VIRTUAL(UZeroExtendeeFilterBase::Passes); return false; }
	
};


