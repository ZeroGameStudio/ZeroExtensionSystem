// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZeroExtensionScope.h"
#include "Scope/ZDeferredExtensionScope.h"

#include "ZeroExtensionSubsystem.generated.h"

UCLASS(NotBlueprintable)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZeroExtensionSubsystem final : public UEngineSubsystem, public IZeroExtensionScope
{
	GENERATED_BODY()

public:
	static UZeroExtensionSubsystem& Get() { return *GEngine->GetEngineSubsystem<UZeroExtensionSubsystem>(); }

public:
	// IZeroExtensionScope
	virtual void ZeroExtensionScope_RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ZeroExtensionScope_UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ZeroExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ZeroExtensionScope_UnregisterExtendee(UObject* extendee, bool pendingKill, FGameplayTag channel = FGameplayTag::EmptyTag) override;

private:
	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	ZES::FZDeferredExtensionScope ExtensionScope;
	
};


