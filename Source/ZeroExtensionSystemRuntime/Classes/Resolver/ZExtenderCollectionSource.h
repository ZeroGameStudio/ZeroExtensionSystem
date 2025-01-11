// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Extender/ZExtenderBaseInterface.h"

#include "ZExtenderCollectionSource.generated.h"

class UZExtenderBaseInterface;

UCLASS(NotBlueprintable, Const)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtenderCollectionSource final : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	const TArray<TObjectPtr<UZExtenderCollectionSource>>& GetPrototypes() const { return Prototypes; }
	const TArray<TObjectPtr<UZExtenderBaseInterface>>& GetExtenders() const { return Extenders; }
	
	void ForeachPrototype(TFunctionRef<void(const UZExtenderCollectionSource*)> action) const;
	void ForeachExtender(TFunctionRef<void(const UZExtenderBaseInterface*)> action) const;
	
private:
	UPROPERTY(EditAnywhere, Category = "Extender")
	TArray<TObjectPtr<UZExtenderCollectionSource>> Prototypes;

	UPROPERTY(EditAnywhere, Category = "Extender")
	TArray<TObjectPtr<UZExtenderBaseInterface>> Extenders;
	
};


