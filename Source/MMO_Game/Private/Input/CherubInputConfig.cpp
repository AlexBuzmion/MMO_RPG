// Alex Buzmion II 2025


#include "Input/CherubInputConfig.h"

const UInputAction* UCherubInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FCherubInputAction& action : AbilityInputActions)
	{
		if (action.InputAction && action.InputTag == InputTag)
		{
			return action.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("Cant find Ability Input Action for Input Tag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	return nullptr; 
}
