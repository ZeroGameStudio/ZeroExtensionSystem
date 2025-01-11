// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderBaseInterface.h"

#include "ZIdentityExtender.generated.h"

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZIdentityExtender final : public UZExtenderBaseInterface
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetExtensionKey() const override { return ExtensionKey; }
	virtual FGameplayTag GetExtensionChannel() const override { return {}; }

protected:
	virtual bool Extend(UObject* extendee) override { return true;}
	virtual bool Revert(UObject* extendee, bool destroying) override { return true; }
	virtual bool CanExtend(UObject* extendee) const override { return false; }

private:
	UPROPERTY(EditAnywhere, Category = "Extender")
	FGameplayTag ExtensionKey;

};


