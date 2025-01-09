// Copyright Zero Games. All Rights Reserved.

#include "Filter/ZScriptableExtendeeFilterBase.h"

bool UZScriptableExtendeeFilterBase::InternalPasses(const UObject* extendee) const
{
	return ScriptPasses(extendee);
}


