// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZeroExtenderBase.h"
#include "ZFilterableExtenderBase.generated.h"

class UZeroExtendeeFilterBase;

UCLASS(Abstract)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZFilterableExtenderBase : public UZeroExtenderBase
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetExtensionKey() const override final { return ExtensionKey; }

private:
	virtual bool CanExtend(UObject* extendee) const override final;

private:
	UPROPERTY(EditAnywhere, Category = "Extender")
	FGameplayTag ExtensionKey;
	
	UPROPERTY(EditAnywhere, Category = "Filter")
	TObjectPtr<UZeroExtendeeFilterBase> Filter;
	
};


