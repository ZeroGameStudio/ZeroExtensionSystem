// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZeroExtensionScope.h"

#include "ZeroExtensionScopeImpl.h"
#include "ZeroExtensionSystemRuntimeLogChannels.h"

TScriptInterface<IZeroExtensionScope> IZeroExtensionScope::Create(UObject* outer, FName name)
{
	return NewObject<UZeroExtensionScopeImpl>(outer, name);
}

bool IZeroExtensionScope::TryClose()
{
	if (ZeroExtensionScope_SupportsClose())
	{
		ZeroExtensionScope_Close();
		return true;
	}

	return false;
}

void IZeroExtensionScope::ZeroExtensionScope_Close()
{
	if (!ensure(ZeroExtensionScope_SupportsClose()))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("Can't close this extension scope (%s) explicitly!"), *_getUObject()->GetName());
	}
}


