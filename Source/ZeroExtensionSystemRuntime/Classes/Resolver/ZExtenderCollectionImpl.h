// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtenderCollection.h"

#include "ZExtenderCollectionImpl.generated.h"

UCLASS(Transient)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtenderCollectionImpl final : public UObject, public IZExtenderCollection
{
	GENERATED_BODY()

public:
	virtual void Register(IZExtensionScope* scope) override;
	virtual void Unregister() override;
	
public:
	void Initialize(const TArray<UZExtenderBaseInterface*>& extenders);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UZExtenderBaseInterface>> Extenders;

	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IZExtensionScope> OwnerScope;
	
private:
	bool bInitialized;
	bool bRegistered;
	
};


