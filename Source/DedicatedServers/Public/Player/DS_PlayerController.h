// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Types/DS_Types.h"
#include "DS_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimerStateChanged, float, Time, ECountdownTimerType, Type);

UCLASS()
class DEDICATEDSERVERS_API ADS_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ADS_PlayerController();

	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChanged OnTimerUpdated;
	UFUNCTION(Client, Reliable)
	void Client_TimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type) const;
	
	UPROPERTY(BlueprintAssignable)
	FOnTimerStateChanged OnTimerStopped;
	UFUNCTION(Client, Reliable)
	void Client_TimerStopped(float CountdownTimeLeft, ECountdownTimerType Type) const;

	virtual void ReceivedPlayer() override;

	UPROPERTY(BlueprintReadOnly)
	FString Username;
	UPROPERTY(BlueprintReadOnly)
	FString PlayerSessionId;

	UFUNCTION(Client, Reliable)
	void TravelToIpAndPort(const FString& ServerAddress, const FString& OptionsParams);
	
protected:
	
	UFUNCTION(Server, Reliable)
	void Server_Ping(float TimeOfRequest);

	UFUNCTION(Client, Reliable)
	void Client_Pong(float TimeOfRequest);
	
private:
	float SingleTripTime;
	
	
};
