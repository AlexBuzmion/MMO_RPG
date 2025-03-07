// Alex Buzmion II 2025


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/Cherub_AttributeSet.h"


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
	// health change binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	// max health change binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	// mana change binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChanged);
	// max mana change binding
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(cAS->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
