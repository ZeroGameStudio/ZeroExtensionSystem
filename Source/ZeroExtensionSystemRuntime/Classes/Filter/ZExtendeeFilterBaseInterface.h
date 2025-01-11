// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtendeeFilterBaseInterface.generated.h"

UCLASS(Abstract, DefaultToInstanced, EditInlineNew, Const, BlueprintType)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtendeeFilterBaseInterface : public UObject
{
	GENERATED_BODY()

public:
	virtual bool Passes(const UObject* extendee) const { PURE_VIRTUAL(UZeroExtendeeFilterBase::Passes); return false; }
	
};


