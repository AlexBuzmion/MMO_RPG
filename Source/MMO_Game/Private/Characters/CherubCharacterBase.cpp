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

void ACherubCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> Effect, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(Effect);
	FGameplayEffectContextHandle contextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	contextHandle.AddSourceObject(this); 
	const FGameplayEffectSpecHandle specHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(Effect, Level, contextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*specHandle.Data.Get(), GetAbilitySystemComponent());
}

void ACherubCharacterBase::InitializeDefaultAttributes() const
{
	// keeping this, and changing depending if we want other attributes driven by primary attributes. 
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1);
	ApplyEffectToSelf(DefaultBasicAttributes, 1);
}


