// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderBase.h"

#include "ZScriptableExtenderBase.generated.h"

UCLASS(Abstract, Blueprintable)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZScriptableExtenderBase final : public UZExtenderBase
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptExtend(UObject* extendee);

	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptRevert(UObject* extendee, bool destroying);

	UFUNCTION(BlueprintImplementableEvent)
	bool ScriptIsTrivialRevert(UObject* extendee, bool destroying) const;
	
private:
	virtual bool Extend(UObject* extendee) override;
	virtual bool Revert(UObject* extendee, bool destroying) override;
	virtual bool IsTrivialRevert(UObject* extendee, bool destroying) const override;
	
};


