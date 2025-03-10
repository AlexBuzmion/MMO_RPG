// Alex Buzmion II 2025


#include "AbilitySystem/Cherub_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Net/UnrealNetwork.h"
#include "CherubGameplayTags.h"

UCherub_AttributeSet::UCherub_AttributeSet()
{
	const FCherubGameplayTags& gameplayTags = FCherubGameplayTags::Get();
	// Primary Attributes
	TagsToAttributes.Add(gameplayTags.Attribute_Primary_Attack, GetAttackAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Primary_MagicAttack, GetMagicAttackAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Primary_Defense, GetDefenseAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Primary_Dexterity, GetDexterityAttribute);

	// Secondary Attributes
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_Agility, GetAgilityAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_Accuracy, GetAccuracyAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_HealthRegeneration, GetHealthRegenAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_ManaRegeneration, GetManaRegenAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_CriticalRate, GetCriticalChanceAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_SpellCriticalRate, GetSpellCriticalChanceAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_CriticalDamage, GetCriticalDamageAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_CriticalResistance, GetCriticalResistanceAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(gameplayTags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);
	
}

void UCherub_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// Basic Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, Mana, COND_None, REPNOTIFY_Always);
	
	// Primary Attributes 
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, Attack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, MagicAttack, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, Defense, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, Dexterity, COND_None, REPNOTIFY_Always);
	
	// Secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, Agility, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, Accuracy, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, HealthRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, ManaRegen, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, CriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, SpellCriticalChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, CriticalDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, CriticalResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCherub_AttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	
}

void UCherub_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	
	// clamping health to max health
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}	
	// clamping mana to max mana
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
	}	
}

void UCherub_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}
}

void UCherub_AttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,
	FEffectProperties& Props) const
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->GetAvatarActor();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = pawn->GetController();
			}
		}
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn()); 
		}
	}
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.GetAvatarActor();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

// Basic Attributes Rep Notify Functions 
void UCherub_AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, Health, OldHealth);
}

void UCherub_AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, Mana, OldMana);
}
// Primary Attributes Rep Notify Functions 
void UCherub_AttributeSet::OnRep_Attack(const FGameplayAttributeData& OldAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, Attack, OldAttack);
}

void UCherub_AttributeSet::OnRep_MagicAttack(const FGameplayAttributeData& OldMagicAttack) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, MagicAttack, OldMagicAttack);
}

void UCherub_AttributeSet::OnRep_Defense(const FGameplayAttributeData& OldDefense) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, Defense, OldDefense);
}

void UCherub_AttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, Dexterity, OldDexterity);
}
// Secondary Attributes Rep Notify Functions 
void UCherub_AttributeSet::OnRep_Agility(const FGameplayAttributeData& OldAgility) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, Agility, OldAgility);
}

void UCherub_AttributeSet::OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, CriticalChance, OldCriticalChance);
}

void UCherub_AttributeSet::OnRep_SpellCriticalChance(const FGameplayAttributeData& OldSpellCriticalChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, SpellCriticalChance, OldSpellCriticalChance);
}

void UCherub_AttributeSet::OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, CriticalDamage, OldCriticalDamage);
}

void UCherub_AttributeSet::OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, CriticalResistance, OldCriticalResistance);
}

void UCherub_AttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, BlockChance, OldBlockChance);
}

void UCherub_AttributeSet::OnRep_Accuracy(const FGameplayAttributeData& OldAccuracy) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, Accuracy, OldAccuracy);
}

void UCherub_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, MaxHealth, OldMaxHealth);
}

void UCherub_AttributeSet::OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, HealthRegen, OldHealthRegen);
}

void UCherub_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, MaxMana, OldMaxMana);
}

void UCherub_AttributeSet::OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCherub_AttributeSet, ManaRegen, OldManaRegen);
}
