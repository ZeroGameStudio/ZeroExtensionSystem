// Copyright Zero Games. All Rights Reserved.

#include "Resolver/ZExtenderCollectionSourceVisitor.h"

#include "GameplayTagContainer.h"
#include "Extender/ZIdentityExtender.h"
#include "Resolver/ZExtenderCollectionSource.h"

namespace ZES::ZExtenderCollectionSourceVisitor_Private
{
	static bool IsPrototypeOf(const UZExtenderCollectionSource* source, const UZExtenderCollectionSource* prototype)
	{
		TSet<UZExtenderCollectionSource*> traversedSources;
		TFunction<bool(const UZExtenderCollectionSource*)> recurse = nullptr;
		recurse = [&](const UZExtenderCollectionSource* current)
		{
			for (const auto& proto : current->GetPrototypes())
			{
				if (traversedSources.Contains(proto))
				{
					continue;
				}

				traversedSources.Emplace(proto);
				
				if (proto == prototype || recurse(proto))
				{
					return true;
				}
			}

			return false;
		};
		
		return recurse(source);
	}
}

ZES::FZExtenderCollectionSourceVisitor::FZExtenderCollectionSourceVisitor(const UZExtenderCollectionSource* source)
	: Source(source)
{
	
}

ZES::EZExtenderCollectionSourceLootResult ZES::FZExtenderCollectionSourceVisitor::LootExtenders(TArray<UZExtenderBaseInterface*>& extenders) const
{
	struct FZRec
	{
		const UZExtenderCollectionSource* Source;
		UZExtenderBaseInterface* Extender;
	};
	
	extenders.Empty();
	
	TSet<const UZExtenderCollectionSource*> traversedSources;
	TMap<FGameplayTag, FZRec> extensionKeyLookup;

	TFunction<EZExtenderCollectionSourceLootResult(const UZExtenderCollectionSource*)> loot = nullptr;
	loot = [&](const UZExtenderCollectionSource* current)
	{
		if (!current)
		{
			return EZExtenderCollectionSourceLootResult::SourceUnavailable;
		}

		if (traversedSources.Contains(current))
		{
			return EZExtenderCollectionSourceLootResult::Cycle;
		}

		traversedSources.Emplace(current);

		for (const auto& prototype : current->GetPrototypes())
		{
			EZExtenderCollectionSourceLootResult res = loot(prototype);
			if (res != EZExtenderCollectionSourceLootResult::Success)
			{
				return res;
			}
		}

		for (const auto& extender : current->GetExtenders())
		{
			if (!IsValid(extender))
			{
				continue;
			}
			
			FGameplayTag extensionKey = extender->GetExtensionKey();
			FZRec& rec = extensionKeyLookup.FindOrAdd(extensionKey);
			if (rec.Source && !ZExtenderCollectionSourceVisitor_Private::IsPrototypeOf(current, rec.Source))
			{
				return EZExtenderCollectionSourceLootResult::Conflict;
			}
			
			rec.Source = current;
			rec.Extender = extender;
		}

		return EZExtenderCollectionSourceLootResult::Success;
	};

	EZExtenderCollectionSourceLootResult res = loot(Source.Get());
	if (res != EZExtenderCollectionSourceLootResult::Success)
	{
		return res;
	}

	extenders.Reserve(extensionKeyLookup.Num());
	for (const auto& pair : extensionKeyLookup)
	{
		extenders.Emplace(pair.Value.Extender);
	}

	return EZExtenderCollectionSourceLootResult::Success;
}


