// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Scope/ZExtensionScope.h"

#include "ZExtensionScopeImpl.generated.h"

UCLASS(MinimalAPI, Transient, NotBlueprintable)
class UZExtensionScopeImpl final : public UObject, public IZExtensionScope
{
	GENERATED_BODY()

private:
	// UObject
	static void AddReferencedObjects(UObject* self, FReferenceCollector& collector);

	virtual void BeginDestroy() override;
	
	// IZeroExtensionScope
	virtual void ExtensionScope_RegisterExtender(UZExtenderBaseInterface* extender) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBaseInterface* extender) override;

	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) override;

	virtual bool ExtensionScope_SupportsClose() const override { return true; }
	virtual void ExtensionScope_Close() override;

private:
	void InternalRegisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channelId);
	void InternalUnregisterExtender(UZExtenderBaseInterface* extender, FGameplayTag channelId);

	void InternalRegisterExtendee(UObject* extendee, FGameplayTag channelId);
	void InternalUnregisterExtendee(UObject* extendee, FGameplayTag channelId, bool destroying);

private:
	struct FZChannel
	{
		explicit FZChannel(FName name): Name(name){}
		
		bool AddExtender(UZExtenderBaseInterface* extender);
		bool RemoveExtender(UZExtenderBaseInterface* extender);
		
		bool AddExtendee(UObject* extendee);
		bool RemoveExtendee(UObject* extendee);

		void ForeachExtender(TFunctionRef<void(UZExtenderBaseInterface*)> action);
		void ForeachExtendee(TFunctionRef<void(UObject*)> action);
		
		void AddReferencedObjects(FReferenceCollector& collector);

	private:
		FName Name;
		TArray<TObjectPtr<UZExtenderBaseInterface>> Extenders;
		TArray<FWeakObjectPtr> Extendees;
		TSet<TObjectKey<UZExtenderBaseInterface>> ExtenderLookup;
		TSet<FObjectKey> ExtendeeLookup;
		bool bExtendersLocked = false;
		bool bExtendeesLocked = false;
	};

private:
	FZChannel& GetChannel(FGameplayTag id);

private:
	FName Name;
	TMap<FGameplayTag, TUniquePtr<FZChannel>> Registry;
	TSet<FGameplayTag> RegisteredExtensionKeys;

	bool bClosed = false;
	
};


