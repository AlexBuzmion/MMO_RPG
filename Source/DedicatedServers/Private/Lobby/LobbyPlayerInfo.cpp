// Alex Buzmion II 2025

#include "DedicatedServers/Public/Lobby/LobbyPlayerInfo.h"

void FLobbyPlayerInfoArray::AddPlayer(const FLobbyPlayerInfo& NewPlayerInfo)
{
	int32 index = Players.Add(NewPlayerInfo);
	MarkItemDirty(Players[index]);
	Players[index].PostReplicatedAdd(*this);
}

void FLobbyPlayerInfoArray::RemovePlayer(const FString& Username)
{
	for (int32 playerIndex = 0; playerIndex < Players.Num(); ++playerIndex)
	{
		FLobbyPlayerInfo& playerInfo = Players[playerIndex];
		if (playerInfo.Username == Username)
		{
			playerInfo.PreReplicatedRemove(*this);
			Players.RemoveAtSwap(playerIndex); // remove at swap takes the last element in the array and replaces the removed element with it instead of moving all elements of the array up by 1 from the removed element
			MarkArrayDirty();
			break; 
		}
	}
}
