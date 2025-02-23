// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Types/DS_Types.h"
#include "DS_GameMode_Base.generated.h"

UCLASS()
class DEDICATEDSERVERS_API ADS_GameMode_Base : public AGameMode
{
	GENERATED_BODY()

protected:
	void StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle);
	void UpdateCountdownTimer(const FCountdownTimerHandle& CountdownTimerHandle);
	
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type);
	virtual void TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap);
	void RemovePlayerSession(AController* Exiting);
};
