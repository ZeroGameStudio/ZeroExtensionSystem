// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderCollection.generated.h"

class IZExtensionScope;
class UZExtenderBaseInterface;

UINTERFACE(MinimalAPI, BlueprintType, meta = (CannotImplementInterfaceInBlueprint))
class UZExtenderCollection : public UInterface
{
	GENERATED_BODY()
};

class ZEROEXTENSIONSYSTEMRUNTIME_API IZExtenderCollection : public IInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void Register(TScriptInterface<IZExtensionScope> scope) = 0;

	UFUNCTION(BlueprintCallable)
	virtual void Unregister() = 0;
	
};


