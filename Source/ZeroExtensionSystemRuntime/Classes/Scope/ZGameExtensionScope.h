// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtensionScope.h"
#include "Scope/ZDeferredExtensionScope.h"

#include "ZGameExtensionScope.generated.h"

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZGameExtensionScope : public UGameInstanceSubsystem, public IZExtensionScope
{
	GENERATED_BODY()

public:
	static UZGameExtensionScope& Get(const UGameInstance* gameInstance) { return *gameInstance->GetSubsystem<UZGameExtensionScope>(); }

public:
	// IZeroExtensionScope
	virtual void ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool pendingKill, FGameplayTag channel = FGameplayTag::EmptyTag) override;

private:
	// USubsystem
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	ZES::FZDeferredExtensionScope ExtensionScope;
	
};


