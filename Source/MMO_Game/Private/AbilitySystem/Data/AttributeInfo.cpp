// Alex Buzmion II 2025


#include "AbilitySystem/Data/AttributeInfo.h"

FCherubAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FCherubAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("No info for Attribute Tag: %s on AttributeInfo: %s"),*AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FCherubAttributeInfo();
}
