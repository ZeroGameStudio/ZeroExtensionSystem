﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderBaseInterface.h"

#include "ZExtenderBase.generated.h"

class UZExtendeeFilterBaseInterface;

UCLASS(Abstract)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtenderBase : public UZExtenderBaseInterface
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
	TObjectPtr<UZExtendeeFilterBaseInterface> Filter;
	
};


