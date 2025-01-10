// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtendeeFilterBase.h"

#include "ZScriptableExtendeeFilterBase.generated.h"

UCLASS(Abstract)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZScriptableExtendeeFilterBase : public UZExtendeeFilterBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptPasses(const UObject* extendee) const;
	
private:
	virtual bool InternalPasses(const UObject* extendee) const override final;
	
};


