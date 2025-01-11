// Copyright Zero Games. All Rights Reserved.

#pragma once

class UZExtenderBaseInterface;
class UZExtenderCollectionSource;

namespace ZES
{
	enum class EZExtenderCollectionSourceLootResult : uint8
	{
		Success,
		Cycle,
		Conflict,
		SourceUnavailable,
	};
	
	class ZEROEXTENSIONSYSTEMRUNTIME_API FZExtenderCollectionSourceVisitor
	{

	public:
		explicit FZExtenderCollectionSourceVisitor(const UZExtenderCollectionSource* source);

	public:
		EZExtenderCollectionSourceLootResult LootExtenders(TArray<UZExtenderBaseInterface*>& extenders) const;

	private:
		TStrongObjectPtr<const UZExtenderCollectionSource> Source;
		
	};
}


