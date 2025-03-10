// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

UCLASS()
class MMO_GAME_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition MagicAttackDefintion;
	float BaseIncrease = 30.0f;
	float AttributeAmplifier = 4; 
};
