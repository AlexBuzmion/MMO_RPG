// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "APIData.generated.h"

UCLASS()
class DEDICATEDSERVERS_API UAPIData : public UDataAsset
{
	GENERATED_BODY()

public:
	FString GetAPIEndpoint(const FGameplayTag& APIEndpoint);
	
protected:
	// name of this API - for labeling in the Data Asset; not used by any code
	UPROPERTY(EditDefaultsOnly)
	FString APIName;

	UPROPERTY(EditDefaultsOnly)
	FString InvokeURL;

	UPROPERTY(EditDefaultsOnly)
	FString StageName;
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FString> Resources; 
};
