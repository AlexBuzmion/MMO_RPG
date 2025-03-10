// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FCherubAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name = FText();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Description = FText();
	UPROPERTY(BlueprintReadOnly)
	float Value = 0.0f;
};

UCLASS()
class MMO_GAME_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	FCherubAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FCherubAttributeInfo> AttributeInformation;
};
