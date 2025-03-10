// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Cherub_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
			GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
			GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
			GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
			GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){};
	// source properties
	FGameplayEffectContextHandle EffectContextHandle;
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;
	
	// target properties
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
	
};

// typedef is specific to the FGameplayAttribute() signature, but TStaticFuncPtr is generic to any signature chosen/set
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
template<class T>
using TStaticFuncPtr = TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr;

/* //! steps to add a stat:
 * declare FGameplayAttributeData stat with a UPROPERTY(ReplicatedUsing = OnRep_Stat)
 * create the function to be called on ReplicatedUsing
 * Include in the LifetimeReplicatedProps
 */
UCLASS()
class MMO_GAME_API UCherub_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UCherub_AttributeSet();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	//! FGameplayAttribute(*)() == TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()> > TagsToAttributes;

private:
	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;

public:
	//! Basic Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Basic AttributeSet")
	FGameplayAttributeData Health;
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Basic AttributeSet")
	FGameplayAttributeData MaxHealth;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Basic AttributeSet")
	FGameplayAttributeData Mana;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Basic AttributeSet")
	FGameplayAttributeData MaxMana;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, MaxMana);

	//! Primary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack, Category = "Primary AttributeSet")
	FGameplayAttributeData Attack;
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldAttack) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, Attack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MagicAttack, Category = "Primary AttributeSet")
	FGameplayAttributeData MagicAttack;
	UFUNCTION()
	void OnRep_MagicAttack(const FGameplayAttributeData& OldMagicAttack) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, MagicAttack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense, Category = "Primary AttributeSet")
	FGameplayAttributeData Defense;
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldDefense) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, Defense);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Dexterity, Category = "Primary AttributeSet")
	FGameplayAttributeData Dexterity;
	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, Dexterity);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Secondary AttributeSet")
	FGameplayAttributeData Agility;
	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldAgility) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, Agility);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Accuracy, Category = "Secondary AttributeSet")
	FGameplayAttributeData Accuracy;
	UFUNCTION()
	void OnRep_Accuracy(const FGameplayAttributeData& OldAccuracy) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, Accuracy);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegen, Category = "Secondary AttributeSet")
	FGameplayAttributeData HealthRegen;
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, HealthRegen);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegen, Category = "Secondary AttributeSet")
	FGameplayAttributeData ManaRegen;
	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, ManaRegen);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Secondary AttributeSet")
	FGameplayAttributeData CriticalChance;
	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldCriticalChance) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, CriticalChance);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_SpellCriticalChance, Category = "Secondary AttributeSet")
	FGameplayAttributeData SpellCriticalChance;
	UFUNCTION()
	void OnRep_SpellCriticalChance(const FGameplayAttributeData& OldSpellCriticalChance) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, SpellCriticalChance);
		
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalDamage, Category = "Secondary AttributeSet")
	FGameplayAttributeData CriticalDamage;
	UFUNCTION()
	void OnRep_CriticalDamage(const FGameplayAttributeData& OldCriticalDamage) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, CriticalDamage);
		
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalResistance, Category = "Secondary AttributeSet")
	FGameplayAttributeData CriticalResistance;
	UFUNCTION()
	void OnRep_CriticalResistance(const FGameplayAttributeData& OldCriticalResistance) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, CriticalResistance);
		
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category = "Secondary AttributeSet")
	FGameplayAttributeData BlockChance;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	ATTRIBUTE_ACCESSORS(UCherub_AttributeSet, BlockChance);
};

 

