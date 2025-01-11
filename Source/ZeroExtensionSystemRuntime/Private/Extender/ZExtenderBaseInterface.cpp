// Copyright Zero Games. All Rights Reserved.

#include "Extender/ZExtenderBaseInterface.h"

#include "Scope/ZExtensionScope.h"

IZExtensionScope* ZES::ZExtenderBaseInterface_Private::FZVisitor::GetOwnerScope() const
{
	return Extender->OwnerScope.GetInterface();
}

bool ZES::ZExtenderBaseInterface_Private::FZVisitor::IsRegistered() const
{
	return Extender->bRegistered;
}

void ZES::ZExtenderBaseInterface_Private::FZVisitor::HandleRegister(IZExtensionScope* scope) const
{
	Extender->bRegistered = true;
	Extender->OwnerScope = scope->_getUObject();
}

void ZES::ZExtenderBaseInterface_Private::FZVisitor::HandleUnregister() const
{
	Extender->OwnerScope = nullptr;
	Extender->MarkAsGarbage();
}

bool UZExtenderBaseInterface::TryExtend(UObject* extendee)
{
	if (!IsValidExtendee(extendee))
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

	destroying |= TreatsGarbageAsPendingKill(extendee) && !IsValidExtendee(extendee);

	if (IsTrivialRevert(extendee, destroying))
	{
		return true;
	}

	// Extender itself should know whether it extended this extendee so we don't check CanExtend() again.
	return Revert(extendee, destroying);
}

bool UZExtenderBaseInterface::IsValidExtendee(const UObject* extendee)
{
	return extendee && !extendee->IsUnreachable() && !extendee->HasAnyFlags(RF_MirroredGarbage);
}


