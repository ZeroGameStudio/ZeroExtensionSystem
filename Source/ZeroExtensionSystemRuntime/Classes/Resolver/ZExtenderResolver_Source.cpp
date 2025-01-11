// Copyright Zero Games. All Rights Reserved.

#include "ZExtenderResolver_Source.h"

#include "ZExtenderCollectionImpl.h"
#include "Extender/ZExtenderBaseInterface.h"
#include "Resolver/ZExtenderCollectionSourceVisitor.h"

TScriptInterface<IZExtenderCollection> UZExtenderResolver_Source::Resolve(UObject* outer) const
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


