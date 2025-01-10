// Copyright Zero Games. All Rights Reserved.


#include "Filter/ZExtendeeFilterBase.h"

bool UZExtendeeFilterBase::Passes(const UObject* extendee) const
{
	return bInversed ^ InternalPasses(extendee);
}


