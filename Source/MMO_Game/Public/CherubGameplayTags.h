// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

// singleton containing native gameplay tags to be able to reference the gameplay tags without using RequestGameplayTag() 

struct FCherubGameplayTags
{
public:
		static const FCherubGameplayTags& Get() { return GameplayTags; }
		static void InitializeNativeGameplayTags();

	FGameplayTag Attribute_Primary_Attack;
	FGameplayTag Attribute_Primary_MagicAttack;
	FGameplayTag Attribute_Primary_Defense;
	FGameplayTag Attribute_Primary_Dexterity;
	
	FGameplayTag Attribute_Secondary_Accuracy;
	FGameplayTag Attribute_Secondary_Agility;
	FGameplayTag Attribute_Secondary_MaxMana; 
	FGameplayTag Attribute_Secondary_MaxHealth; 
	FGameplayTag Attribute_Secondary_HealthRegeneration; 
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_CriticalRate;
	FGameplayTag Attribute_Secondary_SpellCriticalRate;
	FGameplayTag Attribute_Secondary_CriticalDamage;
	FGameplayTag Attribute_Secondary_CriticalResistance;
	FGameplayTag Attribute_Secondary_BlockChance;

	FGameplayTag InputTag_LeftMB;
	FGameplayTag InputTag_RightMB;
	FGameplayTag InputTag_1; 
	FGameplayTag InputTag_2; 
	FGameplayTag InputTag_3; 
	FGameplayTag InputTag_4; 
private:
	static FCherubGameplayTags GameplayTags; 
};