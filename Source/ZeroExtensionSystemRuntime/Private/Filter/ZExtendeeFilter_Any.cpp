// Copyright Zero Games. All Rights Reserved.

#include "Filter/ZExtendeeFilter_Any.h"

bool UZExtendeeFilter_Any::InternalPasses(const UObject* extendee) const
{
	bool res = false;
	for (const auto& filter : InnerFilters)
	{
		if (!!(res |= filter->Passes(extendee)))
		{
			break;
		}
	}
	
	return res;
}


