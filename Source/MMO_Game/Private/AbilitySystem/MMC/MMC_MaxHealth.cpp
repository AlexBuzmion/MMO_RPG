// Alex Buzmion II 2025


#include "AbilitySystem/MMC/MMC_MaxHealth.h"

#include "AbilitySystem/Cherub_AttributeSet.h"
#include "Interface/CombatInterface.h"


UMMC_MaxHealth::UMMC_MaxHealth()
{
	// define the attribute we want to capture
	AttributeDefinition.AttributeToCapture = UCherub_AttributeSet::GetDefenseAttribute();
	AttributeDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// capturing the attribute right away if true, as soon as the effect spec is created
	AttributeDefinition.bSnapshot = false;
	// add to the array of attributes to capture
	RelevantAttributesToCapture.Add(AttributeDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target
	const FGameplayTagContainer* sourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* targetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	// pass the tags 
	FAggregatorEvaluateParameters evaluationParams;
	evaluationParams.SourceTags = sourceTags;
	evaluationParams.TargetTags = targetTags;
	// create the container to store the attribute magnitude
	float attribute = 0.0f;
	GetCapturedAttributeMagnitude(AttributeDefinition, Spec, evaluationParams,attribute);
	// clamp the attribute
	attribute = FMath::Max<float>(attribute, 0.0f);

	ICombatInterface* combatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	const int32 playerLevel = combatInterface->GetActorLevel();
	
	// return the actual value after the custom calculation we desire
	return 80.0f + 2.5f * attribute + 10.0f * playerLevel;
}
