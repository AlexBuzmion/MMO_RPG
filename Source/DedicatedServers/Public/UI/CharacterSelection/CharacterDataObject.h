// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterDataObject.generated.h"

UCLASS()
class DEDICATEDSERVERS_API UCharacterDataObject : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(BlueprintReadWrite)
	FString CharacterClass;

	UPROPERTY(BlueprintReadWrite)
	int32 CharacterLevel;

	UPROPERTY(BlueprintReadWrite)
	FString LastMapLocation;
};
