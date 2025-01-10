// Copyright Zero Games. All Rights Reserved.

#include "Scope/ZExtensionScope.h"

#include "ZExtensionScopeImpl.h"
#include "ZeroExtensionSystemRuntimeLogChannels.h"

TScriptInterface<IZExtensionScope> IZExtensionScope::Create(UObject* outer, FName name)
{
	return NewObject<UZExtensionScopeImpl>(outer, name);
}

bool IZExtensionScope::TryClose()
{
	if (ExtensionScope_SupportsClose())
	{
		ExtensionScope_Close();
		return true;
	}

	return false;
}

void IZExtensionScope::ExtensionScope_Close()
{
	if (!ensure(ExtensionScope_SupportsClose()))
	{
		UE_LOG(LogZeroExtensionSystemRuntime, Warning, TEXT("Can't close this extension scope (%s) explicitly!"), *_getUObject()->GetName());
	}
}


