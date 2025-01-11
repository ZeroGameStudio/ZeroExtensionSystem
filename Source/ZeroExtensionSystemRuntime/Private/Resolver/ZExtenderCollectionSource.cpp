// Copyright Zero Games. All Rights Reserved.

#include "Resolver/ZExtenderCollectionSource.h"

void UZExtenderCollectionSource::ForeachPrototype(TFunctionRef<void(const UZExtenderCollectionSource*)> action) const
{
	for (const auto& prototype : Prototypes)
	{
		action(prototype);
	}
}

void UZExtenderCollectionSource::ForeachExtender(TFunctionRef<void(const UZExtenderBaseInterface*)> action) const
{
	for (const auto& extender : Extenders)
	{
		action(extender);
	}
}


