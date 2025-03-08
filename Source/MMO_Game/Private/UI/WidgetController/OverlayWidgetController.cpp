// Alex Buzmion II 2025


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"
#include "AbilitySystem/Cherub_AttributeSet.h"
#include "Chaos/Deformable/MuscleActivationConstraints.h"


void UOverlayWidgetController::BroadcastInitialValues()
{
	const UCherub_AttributeSet* cAS = Cast<UCherub_AttributeSet>(AttributeSet);
	if (cAS)
	{
		OnHealthChanged.Broadcast(cAS->GetHealth());
		OnMaxHealthChanged.Broadcast(cAS->GetMaxHealth());
		OnManaChanged.Broadcast(cAS->GetMana());
		OnMaxManaChanged.Broadcast(cAS->GetMaxMana());
	}
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UCherub_AttributeSet* cAS = Cast<UCherub_AttributeSet>(AttributeSet);
	// health change lambda binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);		
		}
	);
	// max health change binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetMaxHealthAttribute()).AddLambda(
	[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);
	// mana change binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		}
	);
	// max mana change binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
	);
	// bind a lambda to the EffectAssetTags event
	Cast<UCherub_AbilitySysComponentBase>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for ( const FGameplayTag& tag : AssetTags )
			{
				// "A.1".MatchesTag("A") will return True, "A".MatchesTag("A,1") will return False
				FGameplayTag msgTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				// if it is a message tagbroadcast message 
				if (tag.MatchesTag(msgTag))
				{
					const FUIWidgetRow* row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, tag);
					MessageWidgetRow.Broadcast(*row);
				}
			}
		}
	);
	
}
