// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "ZExtendeeFilterBase.h"

#include "ZExtendeeFilter_WorldType.generated.h"

UENUM(Flags, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EZExtendeeFilter_WorldTypeFlags : uint8
{
	None = 0 UMETA(Hidden),
	
	Game = 1 << 1,
	Editor = 1 << 2,
	PIE = 1 << 3,
	EditorPreview = 1 << 4,
	GamePreview = 1 << 5,
	GameRPC = 1 << 6,
	Inactive = 1 << 7,

	Default = Game | PIE | GamePreview | GameRPC UMETA(Hidden),
};

UCLASS()
class ZEROEXTENSIONSYSTEMRUNTIME_API UZExtendeeFilter_WorldType final : public UZExtendeeFilterBase
{
	GENERATED_BODY()

public:
	virtual bool InternalPasses(const UObject* extendee) const override;

private:
	UPROPERTY(EditAnywhere, Category = "Filter", meta = (Bitmask, BitmaskEnum = "/Script/ZeroExtensionSystemRuntime.EZExtendeeFilter_WorldTypeFlags"))
	uint8 RequiredWorldTypes = static_cast<std::underlying_type_t<EZExtendeeFilter_WorldTypeFlags>>(EZExtendeeFilter_WorldTypeFlags::Default);
	
};


