// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "CherubGameplayAbility.generated.h"

UCLASS()
class MMO_GAME_API UCherubGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Input")
	FGameplayTag StartupInputTag; // these will be set for the class defaults 
	//
	// UPROPERTY(EditDefaultsOnly, Category="Input")
	// FGameplayTag 
};
