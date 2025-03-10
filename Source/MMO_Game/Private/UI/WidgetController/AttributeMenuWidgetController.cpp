// Alex Buzmion II 2025


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "CherubGameplayTags.h"
#include "AbilitySystem/Cherub_AttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UCherub_AttributeSet* attSet = CastChecked<UCherub_AttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (auto& pair : attSet->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(pair.Value()).AddLambda(
			[this, pair](const FOnAttributeChangeData& data)
			{
				BroadcastAttributeInfo(pair.Key, pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UCherub_AttributeSet* attSet = CastChecked<UCherub_AttributeSet>(AttributeSet);
	check(AttributeInfo);
	//! original implementation where it will require broadcasting one at a time per stat
	// bool isFound = false; 
	// FCherubAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(FCherubGameplayTags::Get().Attribute_Primary_Attack, isFound);
	// info.Value = attSet->GetAttack();
	//
	// AttributeInfoDelegate.Broadcast(info);

	for (auto& pair : attSet->TagsToAttributes)
	{
		BroadcastAttributeInfo(pair.Key, pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	bool isFound = false; 
	FCherubAttributeInfo info = AttributeInfo->FindAttributeInfoForTag(AttributeTag, isFound);
	info.Value = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(info);
}
