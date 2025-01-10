// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtendeeFilterBaseInterface.h"
#include "ZExtendeeFilterBase.generated.h"

UCLASS(Abstract)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtendeeFilterBase : public UZExtendeeFilterBaseInterface
{
	GENERATED_BODY()

protected:
	virtual bool InternalPasses(const UObject* extendee) const { PURE_VIRTUAL(UZInversableExtendeeFilterBase::InternalPasses); return false; }
	
private:
	virtual bool Passes(const UObject* extendee) const override final;

private:
	UPROPERTY(EditAnywhere, Category = "Filter")
	bool bInversed;
	
};


