// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "ZExtensionScope.generated.h"

class UZExtenderBase;

UINTERFACE(MinimalAPI)
class UZExtensionScope : public UInterface
{
	GENERATED_BODY()
};

class ZEROEXTENSIONSYSTEMRUNTIME_API IZExtensionScope : public IInterface
{
	GENERATED_BODY()

public:
	static TScriptInterface<IZExtensionScope> Create(UObject* outer, FName name);

public:
	virtual void ExtensionScope_RegisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;
		
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) = 0;

public:
	bool TryClose();

protected:
	virtual bool ExtensionScope_SupportsClose() const { return false; }
	virtual void ExtensionScope_Close();

};


