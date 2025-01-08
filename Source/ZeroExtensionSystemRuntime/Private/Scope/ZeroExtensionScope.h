// Copyright Zero Games. All Rights Reserved.

#pragma once

#include "Scope/IZeroExtensionScope.h"

namespace ZES
{
	class FZeroExtensionScope final : public IZeroExtensionScope, public FGCObject, public FNoncopyable
	{

	public:
		explicit FZeroExtensionScope(FName name) : Name(name){}
		virtual ~FZeroExtensionScope() override;

	private:
		// IZeroExtensionScope
		virtual void RegisterExtender(UZeroExtenderBase* extender, FGameplayTag channel) override;
		virtual void UnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channel) override;

		virtual void RegisterExtendee(UObject* extendee, FGameplayTag channel) override;
		virtual void UnregisterExtendee(UObject* extendee, FGameplayTag channel, bool pendingKill) override;

		virtual FName GetExtensionScopeName() const override { return Name; }

		// FGCObject
		virtual void AddReferencedObjects(FReferenceCollector& collector) override;
		virtual FString GetReferencerName() const override;

	private:
		void InternalRegisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId);
		void InternalUnregisterExtender(UZeroExtenderBase* extender, FGameplayTag channelId);

		void InternalRegisterExtendee(UObject* extendee, FGameplayTag channelId);
		void InternalUnregisterExtendee(UObject* extendee, FGameplayTag channelId, bool pendingKill);

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

		bool bDead = false;
		
	};
}


