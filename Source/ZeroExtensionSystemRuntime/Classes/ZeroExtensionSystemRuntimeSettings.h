// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZeroExtensionSystemRuntimeSettings.generated.h"

UCLASS(Config = ZeroExtensionSystem, DefaultConfig)
class ZEROEXTENSIONSYSTEMRUNTIME_API UZeroExtensionSystemRuntimeSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	bool IsWorldExtensionScopeEnabled() const { return bWorldExtensionScopeEnabled; }
	bool IsPlayerExtensionScopeEnabled() const { return bPlayerExtensionScopeEnabled; }

private:
	UPROPERTY(Config, EditAnywhere, Category = "Scope")
	bool bWorldExtensionScopeEnabled;

	UPROPERTY(Config, EditAnywhere, Category = "Scope")
	bool bPlayerExtensionScopeEnabled;
	
};


