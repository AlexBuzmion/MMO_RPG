// Alex Buzmion II 2025


#include "Player/Cherub_PlayerState.h"

#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"
#include "AbilitySystem/Cherub_AttributeSet.h"

ACherub_PlayerState::ACherub_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCherub_AbilitySysComponentBase>("Ability System Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UCherub_AttributeSet>("AttributeSet");

	SetNetUpdateFrequency(100.0f);
}

UAbilitySystemComponent* ACherub_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
