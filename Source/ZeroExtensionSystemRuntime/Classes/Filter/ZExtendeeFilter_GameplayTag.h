// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"
#include "ZExtendeeFilterBase.h"

#include "ZExtendeeFilter_GameplayTag.generated.h"

UENUM()
enum class EZExtendeeFilter_GameplayTagOperator : uint8
{
	All,
	Any,
};

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtendeeFilter_GameplayTag final : public UZExtendeeFilterBase
{
	GENERATED_BODY()

public:
	virtual bool InternalPasses(const UObject* extendee) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Filter")
	bool bUseQuery;
	
	UPROPERTY(EditAnywhere, Category = "Filter", meta = (EditCondition = "!bUseQuery", EditConditionHides))
	FGameplayTagContainer RequiredTags;

	UPROPERTY(EditAnywhere, Category = "Filter", meta = (EditCondition = "!bUseQuery", EditConditionHides))
	EZExtendeeFilter_GameplayTagOperator Operator;

	UPROPERTY(EditAnywhere, Category = "Filter", meta = (EditCondition = "!bUseQuery", EditConditionHides))
	bool bExactMatch;

	UPROPERTY(EditAnywhere, Category = "Filter", meta = (EditCondition = "bUseQuery", EditConditionHides))
	FGameplayTagQuery Query;
	
};


