// Copyright Zero Games. All Rights Reserved.

#include "Extender/ZExtenderBaseInterface.h"

bool UZExtenderBaseInterface::TryExtend(UObject* extendee)
{
	if (!extendee)
	{
		return false;
	}

	if (!CanExtend(extendee))
	{
		return false;
	}

	return Extend(extendee);
}

bool UZExtenderBaseInterface::TryRevert(UObject* extendee, bool destroying)
{
	if (!extendee)
	{
		return false;
	}

	destroying |= TreatsGarbageAsPendingKill(extendee) && extendee->HasAllFlags(RF_MirroredGarbage);

	if (IsTrivialRevert(extendee, destroying))
	{
		return true;
	}

	if (!CanExtend(extendee))
	{
		return false;
	}

	return Revert(extendee, destroying);
}


