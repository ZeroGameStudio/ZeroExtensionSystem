// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderResolverBaseInterface.generated.h"

class IZExtenderCollection;

UCLASS(Abstract, DefaultToInstanced, EditInlineNew, Const)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtenderResolverBaseInterface : public UObject
{
	GENERATED_BODY()

public:
	virtual TScriptInterface<IZExtenderCollection> Resolve(UObject* outer) const { PURE_VIRTUAL(UZExtenderResolverBaseInterface::Resolve); return nullptr; }

public:
	TScriptInterface<IZExtenderCollection> Resolve() const { return Resolve(nullptr); }
	
};


