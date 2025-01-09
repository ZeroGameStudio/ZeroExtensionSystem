// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Scope/ZeroExtensionScope.h"

#include "ZeroExtensionScopeImpl.generated.h"

UCLASS(MinimalAPI, Transient, NotBlueprintable)
class UZeroExtensionScopeImpl final : public UObject, public IZeroExtensionScope
{
	GENERATED_BODY()

private:
	// UObject
	static void AddReferencedObjects(UObject* self, FReferenceCollector& collector);

	virtual void BeginDestroy() override;
	
	// IZeroExtensionScope
	virtual void ZeroExtensionScope_RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ZeroExtensionScope_UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;

	virtual void ZeroExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ZeroExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) override;

	virtual bool ZeroExtensionScope_SupportsClose() const override { return true; }
	virtual void ZeroExtensionScope_Close() override;

private:
	void InternalRegisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId);
	void InternalUnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId);

	void InternalRegisterExtendee(UObject* extendee, FGameplayTag channelId);
	void InternalUnregisterExtendee(UObject* extendee, FGameplayTag channelId, bool destroying);

private:
	struct FZChannel
	{
		explicit FZChannel(FName name): Name(name){}
		
		bool AddExtender(UZeroExtenderBase* extender);
		bool RemoveExtender(UZeroExtenderBase* extender);
		
		bool AddExtendee(UObject* extendee);
		bool RemoveExtendee(UObject* extendee);

		void ForeachExtender(TFunctionRef<void(UZeroExtenderBase*)> action);
		void ForeachExtendee(TFunctionRef<void(UObject*)> action);
		
		void AddReferencedObjects(FReferenceCollector& collector);

	private:
		FName Name;
		TArray<TObjectPtr<UZeroExtenderBase>> Extenders;
		TArray<FWeakObjectPtr> Extendees;
		TMap<FGameplayTag, TWeakObjectPtr<UZeroExtenderBase>> ExtenderLookup;
		TSet<FObjectKey> ExtendeeLookup;
		bool bExtendersLocked = false;
		bool bExtendeesLocked = false;
	};

private:
	FZChannel& GetChannel(FGameplayTag id);

private:
	FName Name;
	TMap<FGameplayTag, TUniquePtr<FZChannel>> Registry;

	bool bClosed = false;
	
};


