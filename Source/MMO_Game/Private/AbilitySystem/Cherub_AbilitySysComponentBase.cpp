// Alex Buzmion II 2025


#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"

#include "Characters/CherubCharacterBase.h"


void UCherub_AbilitySysComponentBase::AbilityActorInfoSet()
{
	//bind to the delegate for any effect applied to this ability system component
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UCherub_AbilitySysComponentBase::EffectApplied);
}

void UCherub_AbilitySysComponentBase::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                    const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer tagContainer;
	EffectSpec.GetAllAssetTags(tagContainer );
	// broadcast the whole tag container to the widget controller
	EffectAssetTags.Broadcast(tagContainer);
	
}
