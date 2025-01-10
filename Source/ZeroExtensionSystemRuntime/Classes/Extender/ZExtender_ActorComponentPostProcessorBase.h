// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtender_ActorComponentPostProcessorBaseInterface.h"

#include "ZExtender_ActorComponentPostProcessorBase.generated.h"

UCLASS(Abstract)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtender_ActorComponentPostProcessorBase : public UZExtender_ActorComponentPostProcessorBaseInterface
{
	GENERATED_BODY()

private:
	virtual EZExtender_ActorComponentPostProcessorRunningGroup GetRunningGroup() const override final { return RunningGroup; }

private:
	UPROPERTY(EditAnywhere, Category = "Post Processor")
	EZExtender_ActorComponentPostProcessorRunningGroup RunningGroup = EZExtender_ActorComponentPostProcessorRunningGroup::PreRegisterToWorld;
	
};


