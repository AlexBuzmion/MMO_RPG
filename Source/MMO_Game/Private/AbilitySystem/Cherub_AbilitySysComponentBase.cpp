// Alex Buzmion II 2025


#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"

#include "CherubGameplayTags.h"
#include "AbilitySystem/Abilities/CherubGameplayAbility.h"


void UCherub_AbilitySysComponentBase::AbilityActorInfoSet()
{
	//bind to the delegate for any effect applied to this ability system component
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UCherub_AbilitySysComponentBase::EffectApplied);
}

void UCherub_AbilitySysComponentBase::AddCharacterAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto& abilityClass : StartupAbilities )
	{
		FGameplayAbilitySpec abilitySpec = FGameplayAbilitySpec(abilityClass, 1);
		// cast the Ability variable created in the abilitySpec to access the startup input tag
		if (const UCherubGameplayAbility* cGA = Cast<UCherubGameplayAbility>(abilitySpec.Ability))
		{
			// get the ability's dynamic tags and add the input tag of the gameplay ability set to the specific ability
			// dynamic ability tags can be added and removed at run time 
			abilitySpec.GetDynamicSpecSourceTags().AddTag(cGA->StartupInputTag);
			// give the ability to the character
			GiveAbility(abilitySpec);
		}
	}
}

void UCherub_AbilitySysComponentBase::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (auto& abilitySpec : GetActivatableAbilities())
	{
		if ( abilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			if (!abilitySpec.IsActive())
			{
				AbilitySpecInputPressed(abilitySpec);
				TryActivateAbility(abilitySpec.Handle); 
			}
		}
	}
}

void UCherub_AbilitySysComponentBase::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (auto& abilitySpec : GetActivatableAbilities())
	{
		if ( abilitySpec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(abilitySpec);
		}
	}
}

void UCherub_AbilitySysComponentBase::EffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer );
	// broadcast the whole tag container to the widget controller
	EffectAssetTags.Broadcast(tagContainer);
	
}
