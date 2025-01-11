// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderCollection.generated.h"

class IZExtensionScope;
class UZExtenderBaseInterface;

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UZExtenderCollection : public UInterface
{
	GENERATED_BODY()
};

class ZEROEXTENSIONSYSTEMRUNTIME_API IZExtenderCollection : public IInterface
{
	GENERATED_BODY()

public:
	virtual void Register(IZExtensionScope* scope) = 0;
	virtual void Unregister() = 0;
	
};


