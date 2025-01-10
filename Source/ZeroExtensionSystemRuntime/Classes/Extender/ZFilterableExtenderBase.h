// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderBase.h"
#include "ZFilterableExtenderBase.generated.h"

class UZExtendeeFilterBase;

UCLASS(Abstract)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZFilterableExtenderBase : public UZExtenderBase
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
	TObjectPtr<UZExtendeeFilterBase> Filter;
	
};


