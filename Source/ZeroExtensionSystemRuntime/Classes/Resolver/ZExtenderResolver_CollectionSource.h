// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderResolverBaseInterface.h"

#include "ZExtenderResolver_CollectionSource.generated.h"

class UZExtenderCollectionSource;

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtenderResolver_CollectionSource final : public UZExtenderResolverBaseInterface
{
	GENERATED_BODY()

public:
	virtual TScriptInterface<IZExtenderCollection> Resolve(UObject* outer) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Resolver")
	TObjectPtr<UZExtenderCollectionSource> Source;
	
};


