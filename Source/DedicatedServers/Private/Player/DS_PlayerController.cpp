// Alex Buzmion II 2025


#include "Player/DS_PlayerController.h"

ADS_PlayerController::ADS_PlayerController()
{
	SingleTripTime = 0;
	Username = "";
	PlayerSessionId = "";
}

void ADS_PlayerController::Client_TimerUpdated_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	OnTimerUpdated.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}

void ADS_PlayerController::Client_TimerStopped_Implementation(float CountdownTimeLeft, ECountdownTimerType Type) const
{
	OnTimerStopped.Broadcast(CountdownTimeLeft - SingleTripTime, Type);
}

void ADS_PlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	if (GetNetMode() == NM_Standalone) return; // no need to ping the server if there is no server to ping 
	if (IsLocalController())
	{
		Server_Ping(GetWorld()->GetTimeSeconds());
	}
}

void ADS_PlayerController::TravelToIpAndPort_Implementation(const FString& ServerAddress, const FString& OptionsParams)
{
	UE_LOG(LogTemp, Warning, TEXT("Client: Received TestClientRPC on %s"), *OptionsParams);
	ClientTravel(ServerAddress, ETravelType::TRAVEL_Absolute, false);
}

void ADS_PlayerController::Server_Ping_Implementation(float TimeOfRequest)
{
	Client_Pong(TimeOfRequest);
}

void ADS_PlayerController::Client_Pong_Implementation(float TimeOfRequest)
{
	const float roundTripTime = GetWorld()->GetTimeSeconds() - TimeOfRequest;
	SingleTripTime = roundTripTime * 0.5f ;
}
