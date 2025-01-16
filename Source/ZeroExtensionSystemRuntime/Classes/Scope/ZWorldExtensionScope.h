// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtensionScope.h"
#include "Scope/ZDeferredExtensionScope.h"

#include "ZWorldExtensionScope.generated.h"

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZWorldExtensionScope : public UWorldSubsystem, public IZExtensionScope
{
	GENERATED_BODY()

public:
	// IZeroExtensionScope
	virtual void ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool pendingKill, FGameplayTag channel = FGameplayTag::EmptyTag) override;

private:
	// USubsystem
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	ZES::FZDeferredExtensionScope ExtensionScope;
	
};
