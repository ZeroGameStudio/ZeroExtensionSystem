// Copyright Zero Games. All Rights Reserved.

#include "Filter/ZExtendeeFilter_WorldType.h"

bool UZExtendeeFilter_WorldType::InternalPasses(const UObject* extendee) const
{
	UWorld* world = extendee->GetWorld();
	if (!world)
	{
		return false;
	}

	EWorldType::Type worldType = world->WorldType;
	std::underlying_type_t<EZExtendeeFilter_WorldTypeFlags> mask = 1 << worldType;
	return !!(RequiredWorldTypes & mask);
}


