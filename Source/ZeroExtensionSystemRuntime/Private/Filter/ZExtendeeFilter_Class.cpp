// Copyright Zero Games. All Rights Reserved.

#include "Filter/ZExtendeeFilter_Class.h"

bool UZExtendeeFilter_Class::InternalPasses(const UObject* extendee) const
{
	/**
 	 * If extendee is a RequiredClass, then RequiredClass must be in memory already,
 	 * so we just resolve the soft ptr and never load it.
 	 * This prevents us from any loading which can be very very slow...
 	 */
	auto existingClass = RequiredClass.Get();
	if (!existingClass)
	{
		// This means extendee is not a RequiredClass.
		return false;
	}

	return extendee->IsA(existingClass);
}


