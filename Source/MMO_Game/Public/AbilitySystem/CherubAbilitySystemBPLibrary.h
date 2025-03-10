// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CherubAbilitySystemBPLibrary.generated.h"

class UOverlayWidgetComponent;
class UAttributeMenuWidgetController; 
UCLASS()
class MMO_GAME_API UCherubAbilitySystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "CherubAbilitySystemBPLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "CherubAbilitySystemBPLibrary|WidgetController")	
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);
};
