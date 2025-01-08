// Copyright Zero Games. All Rights Reserved.

#include "Scope/IZeroExtensionScope.h"

#include "ZeroExtensionScope.h"

ZES::IZeroExtensionScope* ZES::IZeroExtensionScope::Create(FName name)
{
	return new FZeroExtensionScope { name };
}


