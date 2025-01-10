// Copyright Zero Games. All Rights Reserved.

#include "Extender/ZExtenderBase.h"

#include "Filter/ZExtendeeFilterBaseInterface.h"

bool UZExtenderBase::CanExtend(UObject* extendee) const
{
	// You don't want to extend all objects indiscriminately, so return false if there is no filter.
	return Filter && Filter->Passes(extendee) && InternalCanExtend(extendee);
}


