// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderBase.h"

#include "ZExtender_ActorComponent.generated.h"

class UZExtender_ActorComponentPostProcessorBaseInterface;

UENUM()
enum class EZExtender_ActorComponentRequestCreationCondition : uint8
{
	SkipIfExists,
	SkipIfExistsExactClass,
	AlwaysCreate,
};

USTRUCT()
struct FZExtender_ActorComponentRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<UActorComponent> ComponentClass;

	UPROPERTY(EditAnywhere)
	FName ComponentName;

	UPROPERTY(EditAnywhere)
	EZExtender_ActorComponentRequestCreationCondition CreationCondition = EZExtender_ActorComponentRequestCreationCondition::SkipIfExists;

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UZExtender_ActorComponentPostProcessorBaseInterface>> PostProcessors;
	
};

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtender_ActorComponent final : public UZExtenderBase
{
	GENERATED_BODY()

private:
	virtual bool Extend(UObject* extendee) override;
	virtual bool Revert(UObject* extendee, bool destroying) override;
	virtual bool InternalCanExtend(UObject* extendee) const override;

private:
	void ApplyRequest(AActor* extendee, const FZExtender_ActorComponentRequest& request);

private:
	UPROPERTY(EditAnywhere, Category = "Extender")
	TArray<FZExtender_ActorComponentRequest> Requests;

private:
	TMap<TObjectKey<AActor>, TArray<TWeakObjectPtr<UActorComponent>>> Registry;
	
};


