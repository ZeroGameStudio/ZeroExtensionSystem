// Copyright Zero Games. All Rights Reserved.

#include "Filter/ZExtendeeFilter_GameplayTag.h"

#include "GameplayTagAssetInterface.h"

bool UZExtendeeFilter_GameplayTag::InternalPasses(const UObject* extendee) const
{
	auto tagOwner = Cast<const IGameplayTagAssetInterface>(extendee);
	if (!tagOwner)
	{
		return false;
	}

	FGameplayTagContainer ownedTags;
	tagOwner->GetOwnedGameplayTags(ownedTags);

	if (bUseQuery)
	{
		return ownedTags.MatchesQuery(Query);
	}
	else
	{
		bool res = false;
		switch (Operator)
		{
		case EZExtendeeFilter_GameplayTagOperator::All:
			{
				res = bExactMatch ? ownedTags.HasAllExact(RequiredTags) : ownedTags.HasAll(RequiredTags);
				break;
			}
		case EZExtendeeFilter_GameplayTagOperator::Any:
			{
				res = bExactMatch ? ownedTags.HasAnyExact(RequiredTags) : ownedTags.HasAny(RequiredTags);
				break;
			}
		default:
			{
				break;
			}
		}

		return res;
	}
}


