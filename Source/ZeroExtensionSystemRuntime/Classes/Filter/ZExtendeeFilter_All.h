﻿// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtendeeFilterBase.h"

#include "ZExtendeeFilter_All.generated.h"

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtendeeFilter_All final : public UZExtendeeFilterBase
{
	GENERATED_BODY()

public:
	virtual bool InternalPasses(const UObject* extendee) const override;
	
private:
	UPROPERTY(EditAnywhere, Category = "Filter")
	TArray<TObjectPtr<UZExtendeeFilterBaseInterface>> InnerFilters;
	
};


