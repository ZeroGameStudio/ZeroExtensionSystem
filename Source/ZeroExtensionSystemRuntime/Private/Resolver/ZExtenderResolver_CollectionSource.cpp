// Copyright Zero Games. All Rights Reserved.

#include "ZExtenderResolver_CollectionSource.h"

#include "ZExtenderCollectionImpl.h"
#include "Extender/ZExtenderBaseInterface.h"
#include "Resolver/ZExtenderCollectionSourceVisitor.h"

TScriptInterface<IZExtenderCollection> UZExtenderResolver_CollectionSource::Resolve(UObject* outer) const
{
	TArray<UZExtenderBaseInterface*> extenders;
	if (!ensureAlways(ZES::FZExtenderCollectionSourceVisitor { Source }.LootExtenders(extenders) == ZES::EZExtenderCollectionSourceLootResult::Success))
	{
		return nullptr;
	}

	if (!outer)
	{
		outer = GetTransientPackage();
	}

	auto collection = NewObject<UZExtenderCollectionImpl>(outer);
	collection->Initialize(extenders);

	return collection;
}


