// Alex Buzmion II 2025


#include "Lobby/LobbyState.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ALobbyState::ALobbyState()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ALobbyState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyState, PlayerInfoArray)
}

void ALobbyState::AddPlayerInfo(const FLobbyPlayerInfo& PlayerInfo)
{
	PlayerInfoArray.AddPlayer(PlayerInfo);
}

void ALobbyState::RemovePlayerInfo(const FString& Username)
{
	PlayerInfoArray.RemovePlayer(Username);
}

TArray<FLobbyPlayerInfo> ALobbyState::GetPlayers() const
{
	return PlayerInfoArray.Players;
}

void ALobbyState::OnRep_LobbyPlayerInfo()
{
	FLobbyPlayerInfoDelta delta = ComputePlayerInfoDelta(LastPlayerInfoArray.Players, PlayerInfoArray.Players);
	for (const auto& playerInfo : delta.AddedPlayers)
	{
		OnPlayerInfoAdded.Broadcast(playerInfo);
	}
	for (const auto& playerInfo : delta.RemovedPlayers)
	{
		OnPlayerInfoRemoved.Broadcast(playerInfo);
	}
	
	LastPlayerInfoArray = PlayerInfoArray;
}

FLobbyPlayerInfoDelta ALobbyState::ComputePlayerInfoDelta(const TArray<FLobbyPlayerInfo>& OldArray,
	const TArray<FLobbyPlayerInfo>& NewArray)
{
	FLobbyPlayerInfoDelta delta; 

	TMap<FString, const FLobbyPlayerInfo*> oldMap; 
	TMap<FString, const FLobbyPlayerInfo*> newMap; 

	for (const auto& playerInfo : OldArray)
	{
		oldMap.Add(playerInfo.Username, &playerInfo);
	}
	
	for (const auto& playerInfo : NewArray)
	{
		newMap.Add(playerInfo.Username, &playerInfo);
	}
	for (const auto& oldPlayerInfo : OldArray)
	{
		if (!newMap.Contains(oldPlayerInfo.Username))
		{
			delta.RemovedPlayers.Add(oldPlayerInfo);
		}
	}
	for (const auto& newPlayerInfo : NewArray)
	{
		if (!oldMap.Contains(newPlayerInfo.Username))
		{
			delta.AddedPlayers.Add(newPlayerInfo);
		}
	}
	
	return delta; 
}



