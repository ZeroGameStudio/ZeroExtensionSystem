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
	virtual void ExtensionScope_RegisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtender(UZExtenderBase* extender, FGameplayTag channel = FGameplayTag::EmptyTag) override;

	virtual void ExtensionScope_RegisterExtendee(UObject* extendee, FGameplayTag channel = FGameplayTag::EmptyTag) override;
	virtual void ExtensionScope_UnregisterExtendee(UObject* extendee, bool destroying, FGameplayTag channel = FGameplayTag::EmptyTag) override;

	virtual bool ExtensionScope_SupportsClose() const override { return true; }
	virtual void ExtensionScope_Close() override;

private:
	void InternalRegisterExtender(UZExtenderBase* extender, FGameplayTag channelId);
	void InternalUnregisterExtender(UZExtenderBase* extender, FGameplayTag channelId);

	void InternalRegisterExtendee(UObject* extendee, FGameplayTag channelId);
	void InternalUnregisterExtendee(UObject* extendee, FGameplayTag channelId, bool destroying);

private:
	struct FZChannel
	{
		explicit FZChannel(FName name): Name(name){}
		
		bool AddExtender(UZExtenderBase* extender);
		bool RemoveExtender(UZExtenderBase* extender);
		
		bool AddExtendee(UObject* extendee);
		bool RemoveExtendee(UObject* extendee);

		void ForeachExtender(TFunctionRef<void(UZExtenderBase*)> action);
		void ForeachExtendee(TFunctionRef<void(UObject*)> action);
		
		void AddReferencedObjects(FReferenceCollector& collector);

	private:
		FName Name;
		TArray<TObjectPtr<UZExtenderBase>> Extenders;
		TArray<FWeakObjectPtr> Extendees;
		TMap<FGameplayTag, TWeakObjectPtr<UZExtenderBase>> ExtenderLookup;
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


