// Alex Buzmion II 2025


#include "AbilitySystem/MMC/MMC_MaxMana.h"

#include "AbilitySystem/Cherub_AttributeSet.h"
#include "Interface/CombatInterface.h"
//! refer to MMC_MaxHealth for the code logic explained
UMMC_MaxMana::UMMC_MaxMana()
{
	MagicAttackDefintion.AttributeToCapture = UCherub_AttributeSet::GetMagicAttackAttribute();
	MagicAttackDefintion.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MagicAttackDefintion.bSnapshot = false;
	RelevantAttributesToCapture.Add(MagicAttackDefintion);
	BaseIncrease = 30.0f;
	AttributeAmplifier = 4;
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters evaluationParams;
	evaluationParams.SourceTags = sourceTags;
	evaluationParams.TargetTags = targetTags;

	float attribute = 0.0f;
	GetCapturedAttributeMagnitude(MagicAttackDefintion, Spec, evaluationParams, attribute);

	return BaseIncrease + (attribute * AttributeAmplifier); 
}
