// Alex Buzmion II 2025

#include "Characters/CherubCharacterBase.h"


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

