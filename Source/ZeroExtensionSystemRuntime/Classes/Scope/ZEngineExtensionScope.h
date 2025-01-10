// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtensionScope.h"
#include "Scope/ZDeferredExtensionScope.h"

#include "ZEngineExtensionScope.generated.h"

UCLASS(NotBlueprintable)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZEngineExtensionScope final : public UEngineSubsystem, public IZExtensionScope
{
	GENERATED_BODY()

public:
	static UZEngineExtensionScope& Get() { return *GEngine->GetEngineSubsystem<UZEngineExtensionScope>(); }

public:
	// IZeroExtensionScope
	virtual void ExtensionScope_RegisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool pendingKill, FGameplayTag channel = FGameplayTag::EmptyTag) override;

private:
	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	ZES::FZDeferredExtensionScope ExtensionScope;
	
};


