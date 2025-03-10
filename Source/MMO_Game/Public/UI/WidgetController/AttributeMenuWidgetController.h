// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Cherub_WidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

class UAttributeInfo;
struct FCherubAttributeInfo;
struct FGameplayTag;
struct FGameplayAttribute;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FCherubAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class MMO_GAME_API UAttributeMenuWidgetController : public UCherub_WidgetController
{
	GENERATED_BODY()

public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
