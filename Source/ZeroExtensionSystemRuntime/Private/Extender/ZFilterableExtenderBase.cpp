// Copyright Zero Games. All Rights Reserved.

#include "Extender/ZFilterableExtenderBase.h"

#include "Filter/ZeroExtendeeFilterBase.h"

bool UZFilterableExtenderBase::CanExtend(UObject* extendee) const
{
	// You don't want to extend all objects indiscriminately, so return false if there is no filter.
	return Filter && Filter->Passes(extendee);
}


