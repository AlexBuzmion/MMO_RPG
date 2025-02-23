// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "DS_GameMode_Base.h"
#include "DS_MatchGameMode.generated.h"

UCLASS()
class DEDICATEDSERVERS_API ADS_MatchGameMode : public ADS_GameMode_Base
{
	GENERATED_BODY()

public:
	ADS_MatchGameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	
	UPROPERTY(BlueprintReadWrite)
	EMatchStatus MatchStatus;
	
protected:
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	
	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PreMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle MatchTimer;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle PostMatchTimer;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> LobbyMap; 
};
