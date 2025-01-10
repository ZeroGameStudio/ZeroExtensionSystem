// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "ZExtenderCollectionSource.generated.h"

class UZExtenderBaseInterface;

UCLASS(NotBlueprintable)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtenderCollectionSource final : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Extender")
	TArray<TObjectPtr<UZExtenderCollectionSource>> Prototypes;

	UPROPERTY(EditAnywhere, Category = "Extender")
	TArray<TObjectPtr<UZExtenderBaseInterface>> Extenders;

	UPROPERTY(EditAnywhere, Category = "Extender")
	FGameplayTagContainer EnabledExtensionKeys;

	UPROPERTY(EditAnywhere, Category = "Extender")
	FGameplayTagContainer DisabledExtensionKeys;
	
};


