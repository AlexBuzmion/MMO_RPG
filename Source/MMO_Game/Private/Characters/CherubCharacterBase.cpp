// Alex Buzmion II 2025

#include "Characters/CherubCharacterBase.h"

#include "AbilitySystemComponent.h"


ACherubCharacterBase::ACherubCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

UAbilitySystemComponent* ACherubCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ACherubCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACherubCharacterBase::InitAbilityActorInfo()
{
}

void ACherubCharacterBase::InitializePrimaryAttributes() const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(DefaultPrimaryAttributes);
	const FGameplayEffectContextHandle contextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	const FGameplayEffectSpecHandle specHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(DefaultPrimaryAttributes, 1, contextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), GetAbilitySystemComponent());
}

