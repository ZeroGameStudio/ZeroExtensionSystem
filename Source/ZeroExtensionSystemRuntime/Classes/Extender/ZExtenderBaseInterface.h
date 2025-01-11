// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

#include "ZExtenderBaseInterface.generated.h"

class IZExtensionScope;
class UZExtenderBaseInterface;

namespace ZES::ZExtenderBaseInterface_Private
{
	struct FZVisitor
	{
		friend class UZExtensionScopeImpl;
	private:
		FZVisitor(UZExtenderBaseInterface* extender) : Extender(extender){}
		IZExtensionScope* GetOwnerScope() const;
		bool IsRegistered() const;
		void HandleRegister(IZExtensionScope* scope) const;
		void HandleUnregister() const;
		UZExtenderBaseInterface* Extender;
	};
}

UCLASS(MinimalAPI, Abstract, DefaultToInstanced, EditInlineNew)
class UZExtenderBaseInterface : public UObject
{
	GENERATED_BODY()

	friend ZES::ZExtenderBaseInterface_Private::FZVisitor;

public:
	bool TryExtend(UObject* extendee);
	bool TryRevert(UObject* extendee, bool destroying);

	virtual FGameplayTag GetExtensionKey() const { PURE_VIRTUAL(UZeroExtenderBase::GetExtensionKey); return {}; }

protected:
	virtual bool Extend(UObject* extendee) { PURE_VIRTUAL(UZeroExtenderBase::Extend); return false; }
	virtual bool Revert(UObject* extendee, bool destroying) { PURE_VIRTUAL(UZeroExtenderBase::Revert); return false; }

	virtual bool CanExtend(UObject* extendee) const { PURE_VIRTUAL(UZeroExtenderBase::CanExtend); return false; }
	ZEROEXTENSIONSYSTEMRUNTIME_API virtual bool IsTrivialRevert(UObject* extendee, bool destroying) const { return false; }
	ZEROEXTENSIONSYSTEMRUNTIME_API virtual bool TreatsGarbageAsPendingKill(UObject* extendee) const { return true; }

private:
	static bool IsValidExtendee(const UObject* extendee);

private:
	UPROPERTY(VisibleAnywhere)
	TScriptInterface<IZExtensionScope> OwnerScope;

	UPROPERTY(VisibleAnywhere)
	bool bRegistered;
	
};


