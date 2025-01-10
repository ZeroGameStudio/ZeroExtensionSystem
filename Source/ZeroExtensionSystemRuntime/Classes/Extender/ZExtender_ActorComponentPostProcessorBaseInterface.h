// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtender_ActorComponentPostProcessorBaseInterface.generated.h"

UENUM()
enum class EZExtender_ActorComponentPostProcessorRunningGroup : uint8
{
	PostInitProperties,
	PreRegisterToWorld,
	PostRegisterToWorld,
};

UCLASS(MinimalAPI, Abstract, DefaultToInstanced, EditInlineNew, Const)
class UZExtender_ActorComponentPostProcessorBaseInterface : public UObject
{
	GENERATED_BODY()

public:
	virtual void Apply(UActorComponent* component) const { PURE_VIRTUAL(UZExtender_ActorComponentPostProcessorBaseInterface::Apply); }

	ZEROEXTENSIONSYSTEMRUNTIME_API virtual EZExtender_ActorComponentPostProcessorRunningGroup GetRunningGroup() const { return EZExtender_ActorComponentPostProcessorRunningGroup::PreRegisterToWorld; }
	
};


