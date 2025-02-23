// Alex Buzmion II 2025


#include "Game/DS_MatchGameMode.h"

ADS_MatchGameMode::ADS_MatchGameMode()
{
	bUseSeamlessTravel = true;
	MatchStatus = EMatchStatus::WaitingForPlayers;
	PreMatchTimer.Type = ECountdownTimerType::PreMatch;
	MatchTimer.Type = ECountdownTimerType::InMatch;
	PostMatchTimer.Type = ECountdownTimerType::PostMatch;
}

void ADS_MatchGameMode::PostLogin(APlayerController* NewPlayer) // does not get called for players seamlessly travelling
{
	Super::PostLogin(NewPlayer);
	// if (MatchStatus == EMatchStatus::WaitingForPlayers)
	// {
	// 	MatchStatus = EMatchStatus::PreMatch;
	// 	StartCountdownTimer(PreMatchTimer);
	// }
}

void ADS_MatchGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	RemovePlayerSession(Exiting);
}

void ADS_MatchGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	// if (MatchStatus == EMatchStatus::WaitingForPlayers)
	// {
	// 	MatchStatus = EMatchStatus::PreMatch;
	// 	StartCountdownTimer(PreMatchTimer);
	// }
}

void ADS_MatchGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);

	if (Type == ECountdownTimerType::PreMatch)
	{
		StopCountdownTimer(PreMatchTimer);
		MatchStatus = EMatchStatus::Match;
		StartCountdownTimer(MatchTimer);
	}
	if (Type == ECountdownTimerType::InMatch)
	{
		StopCountdownTimer(MatchTimer);
		MatchStatus = EMatchStatus::PostMatch;
		StartCountdownTimer(PostMatchTimer);
	}
	if (Type == ECountdownTimerType::PostMatch)
	{
		StopCountdownTimer(PostMatchTimer);
		MatchStatus = EMatchStatus::SeamlessTravelling;
		TrySeamlessTravel(LobbyMap);
	}
}
