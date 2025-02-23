// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DS_MatchPlayerState.generated.h"


struct FDS_RecordPlayerDataInput;
class UPlayerDataManager;

UCLASS()
class DEDICATEDSERVERS_API ADS_MatchPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void OnMatchEnded(const FString& Username);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPlayerDataManager> GetPlayerDataManagerClass;

protected:
	virtual void BeginPlay() override;
	void RecordPlayerData(const FDS_RecordPlayerDataInput& PlayerDataInput) const;
	
private:

	UPROPERTY()
	TObjectPtr<UPlayerDataManager> PlayerDataManager;
};
