// Alex Buzmion II 2025


#include "Player/Cherub_PlayerState.h"

#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"
#include "AbilitySystem/Cherub_AttributeSet.h"
#include "Net/UnrealNetwork.h"

ACherub_PlayerState::ACherub_PlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UCherub_AbilitySysComponentBase>("Ability System Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UCherub_AttributeSet>("AttributeSet");

	SetNetUpdateFrequency(100.0f);
}

void ACherub_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACherub_PlayerState, Level); 
}

UAbilitySystemComponent* ACherub_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACherub_PlayerState::OnRep_Level(int32 OldLevel)
{
	
}
