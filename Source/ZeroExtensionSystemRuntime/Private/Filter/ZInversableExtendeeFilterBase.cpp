// Copyright Zero Games. All Rights Reserved.


#include "Filter/ZInversableExtendeeFilterBase.h"

bool UZInversableExtendeeFilterBase::Passes(const UObject* extendee) const
{
	return bInversed ^ InternalPasses(extendee);
}


