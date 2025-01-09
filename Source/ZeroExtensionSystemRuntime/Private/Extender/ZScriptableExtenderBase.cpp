// Copyright Zero Games. All Rights Reserved.

#include "Extender/ZScriptableExtenderBase.h"

bool UZScriptableExtenderBase::Extend(UObject* extendee)
{
	return ScriptExtend(extendee);
}

bool UZScriptableExtenderBase::Revert(UObject* extendee, bool destroying)
{
	return ScriptRevert(extendee, destroying);
}

bool UZScriptableExtenderBase::IsTrivialRevert(UObject* extendee, bool destroying) const
{
	return ScriptIsTrivialRevert(extendee, destroying);
}


