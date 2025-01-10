// Copyright Zero Games. All Rights Reserved.

#include "Filter/ZExtendeeFilter_All.h"

bool UZExtendeeFilter_All::InternalPasses(const UObject* extendee) const
{
	bool res = true;
	for (const auto& filter : InnerFilters)
	{
		if (!(res &= filter->Passes(extendee)))
		{
			break;
		}
	}
	
	return res;
}


