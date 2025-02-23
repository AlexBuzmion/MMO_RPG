// Alex Buzmion II 2025


#include "DS_MatchPlayerState.h"

#include "UI/PlayerData/PlayerDataManager.h"

void ADS_MatchPlayerState::OnMatchEnded(const FString& Username)
{
}

void ADS_MatchPlayerState::BeginPlay()
{
	Super::BeginPlay();
	PlayerDataManager = NewObject<UPlayerDataManager>(this, GetPlayerDataManagerClass); 
}

void ADS_MatchPlayerState::RecordPlayerData(const FDS_RecordPlayerDataInput& PlayerDataInput) const
{
	check(IsValid(PlayerDataManager));
	PlayerDataManager->RecordPlayerData(PlayerDataInput);
}
