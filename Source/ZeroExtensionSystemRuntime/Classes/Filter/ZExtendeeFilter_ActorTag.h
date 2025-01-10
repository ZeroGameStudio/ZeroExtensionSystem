// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtendeeFilterBase.h"

#include "ZExtendeeFilter_ActorTag.generated.h"

UENUM()
enum class EZExtendeeFilter_ActorTagMatchType : uint8
{
	Exact,
	Subsequence,
	Regex,
};

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtendeeFilter_ActorTag final : public UZExtendeeFilterBase
{
	GENERATED_BODY()

public:
	virtual bool InternalPasses(const UObject* extendee) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Filter")
	FName RequiredTag;
	
	UPROPERTY(EditAnywhere, Category = "Filter")
	EZExtendeeFilter_ActorTagMatchType MatchType;
	
};
