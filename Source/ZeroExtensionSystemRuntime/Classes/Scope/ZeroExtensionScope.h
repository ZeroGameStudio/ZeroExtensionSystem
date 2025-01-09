// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "ZeroExtensionScope.generated.h"

class UZeroExtenderBase;

UINTERFACE(MinimalAPI)
class UZeroExtensionScope : public UInterface
{
	GENERATED_BODY()
};

class ZEROEXTENSIONSYSTEMRUNTIME_API IZeroExtensionScope : public IInterface
{
	GENERATED_BODY()

public:
	static TScriptInterface<IZeroExtensionScope> Create(UObject* outer, FName name);

public:
	virtual void ZeroExtensionScope_RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;
	virtual void ZeroExtensionScope_UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;
		
	virtual void ZeroExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;
	virtual void ZeroExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;

public:
	bool TryClose();

protected:
	virtual bool ZeroExtensionScope_SupportsClose() const { return false; }
	virtual void ZeroExtensionScope_Close();

};


