// Copyright Zero Games. All Rights Reserved.

#include "Filter/ZExtendeeFilter_ActorTag.h"

bool UZExtendeeFilter_ActorTag::InternalPasses(const UObject* extendee) const
{
	auto actor = Cast<const AActor>(extendee);
	if (!actor)
	{
		return false;
	}
	
	bool res = false;
	switch (MatchType)
	{
	case EZExtendeeFilter_ActorTagMatchType::Exact:
		{
			res = actor->ActorHasTag(RequiredTag);
			break;
		}
	case EZExtendeeFilter_ActorTagMatchType::Subsequence:
		{
			for (const auto& tag : actor->Tags)
			{
				if (tag.ToString().Contains(RequiredTag.ToString()))
				{
					res = true;
					break;
				}
			}
			break;
		}
	case EZExtendeeFilter_ActorTagMatchType::Regex:
		{
			TStringBuilder<32> sb;
			sb << TEXT("^") << RequiredTag << TEXT("$");
			FRegexPattern pattern { sb.ToString() };
			for (const auto& tag : actor->Tags)
			{
				FRegexMatcher matcher { pattern, tag.ToString() };
				if (matcher.FindNext())
				{
					res = true;
					break;
				}
			}
			break;
		}
	default:
		{
			break;
		}
	}

	return res;
}


