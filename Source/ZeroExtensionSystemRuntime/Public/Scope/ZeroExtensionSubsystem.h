// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "IZeroExtensionScope.h"

#include "ZeroExtensionSubsystem.generated.h"

UCLASS(NotBlueprintable)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZeroExtensionSubsystem final : public UEngineSubsystem, public ZES::IZeroExtensionScope
{
	GENERATED_BODY()

public:
	static UZeroExtensionSubsystem& Get() { return *GEngine->GetEngineSubsystem<UZeroExtensionSubsystem>(); }

public:
	// IZeroExtensionScope
	UFUNCTION(BlueprintCallable)
	virtual void RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel) override;

	UFUNCTION(BlueprintCallable)
	virtual void UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel) override;

	UFUNCTION(BlueprintCallable)
	virtual void RegisterExtendee(UObject* extendee, FGameplayTag channel) override;

	UFUNCTION(BlueprintCallable)
	virtual void UnregisterExtendee(UObject* extendee, FGameplayTag channel, bool pendingKill) override;

	UFUNCTION(BlueprintPure)
	virtual FName GetExtensionScopeName() const override { return GetFName(); }

private:
	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	TUniquePtr<IZeroExtensionScope> InnerExtensionScope;
	
};


