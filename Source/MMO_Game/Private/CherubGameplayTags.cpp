// Alex Buzmion II 2025

#include "CherubGameplayTags.h"
#include "GameplayTagsManager.h"

FCherubGameplayTags FCherubGameplayTags::GameplayTags;

void FCherubGameplayTags::InitializeNativeGameplayTags()
{
	//! Input Tags
	GameplayTags.InputTag_LeftMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LeftMouse"),
		FString("Left mouse click")
	);
	GameplayTags.InputTag_RightMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RightMouse"),
		FString("Right mouse click")
	);
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.1"),
		FString("Key 1 press")
	);
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.2"),
		FString("Key 2 press")
		);
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.3"),
		FString("Key 3 press")
		);
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.4"),
		FString("Key 4 press")
	);
	
	//! Primary Attributes
	GameplayTags.Attribute_Primary_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Attack"),
		FString("increases physical damage")
	);	
	GameplayTags.Attribute_Primary_MagicAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.MagicAttack"),
		FString("increases magical damage")
	);
	GameplayTags.Attribute_Primary_Defense = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Defense"),
		FString("reduces incoming physical damage")
	);
	GameplayTags.Attribute_Primary_Dexterity = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Dexterity"),
		FString("boosts crit potential, accuracy, and evasion")
	);

	//! Secondary Attributes
	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("increases total health capacity")
	);
	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("increases total mana pool")
	);
	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("restores health over time")
	);
	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("restores mana over time")
	);
	GameplayTags.Attribute_Secondary_CriticalRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalRate"),
		FString("increases chance of critical hits")
	);
	GameplayTags.Attribute_Secondary_SpellCriticalRate = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.SpellCriticalRate"),
		FString("increases chance of spell criticals")
	);
	GameplayTags.Attribute_Secondary_CriticalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalDamage"),
		FString("amplifies damage on critical hits")
	);
	GameplayTags.Attribute_Secondary_CriticalResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalResistance"),
		FString("lessens impact of enemy criticals")
	);
	GameplayTags.Attribute_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.BlockChance"),
		FString("increases chance to block incoming attacks")
	);
	GameplayTags.Attribute_Secondary_Accuracy = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Accuracy"),
		FString("improves chance to land hits")
	);
	GameplayTags.Attribute_Secondary_Agility = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Agility"),
		FString("raises movement/attack speed and dodge rate")
	);
	
}
