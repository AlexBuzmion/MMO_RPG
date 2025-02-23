// Alex Buzmion II 2025


#include "UI/Lobby/LobbyPlayerEntryList.h"

#include "Components/ScrollBox.h"
#include "Game/DS_GameState.h"
#include "Lobby/LobbyState.h"
#include "Lobby/LobbyPlayerInfo.h"
#include "UI/Lobby/LobbyPlayerEntry.h"

void ULobbyPlayerEntryList::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ADS_GameState* dsGameState = GetWorld()->GetGameState<ADS_GameState>();
	if (!IsValid(dsGameState)) return;

	if (IsValid(dsGameState->LobbyState)) // if already valid/constructed, bind to the delegates right away
	{
		OnLobbyStateInitialized(dsGameState->LobbyState);
	} else // bind to the OnLobbyStateInitialized to assign the callback to retry accessing the lobby state again 
	{
		dsGameState->OnLobbyStateInitialized.AddDynamic(this, &ULobbyPlayerEntryList::OnLobbyStateInitialized);
	}
}

void ULobbyPlayerEntryList::UpdatePlayerInfo(ALobbyState* LobbyState)
{
	ScrollBox_PlayerEntryList->ClearChildren();
	for (const FLobbyPlayerInfo& playerInfo : LobbyState->GetPlayers())
	{
		CreateAndAddPlayerEntry(playerInfo);
	}
}

ULobbyPlayerEntry* ULobbyPlayerEntryList::FindPlayerEntry(const FString& Username)
{
	for (UWidget* child : ScrollBox_PlayerEntryList->GetAllChildren())
	{
		ULobbyPlayerEntry* playerEntry = Cast<ULobbyPlayerEntry>(child);
		if (IsValid(playerEntry) && playerEntry->GetUsername() == Username)
		{
			return playerEntry;
		}
	}
	return nullptr;
}

void ULobbyPlayerEntryList::OnLobbyStateInitialized(ALobbyState* LobbyState)
{
	if (!IsValid(LobbyState)) return;

	LobbyState->OnPlayerInfoAdded.AddDynamic(this, &ULobbyPlayerEntryList::CreateAndAddPlayerEntry);
	LobbyState->OnPlayerInfoRemoved.AddDynamic(this, &ULobbyPlayerEntryList::OnPlayerRemoved);
	UpdatePlayerInfo(LobbyState);
}

void ULobbyPlayerEntryList::CreateAndAddPlayerEntry(const FLobbyPlayerInfo& PlayerInfo)
{
	if (FindPlayerEntry(PlayerInfo.Username)) return;
	
	ULobbyPlayerEntry* playerEntry = CreateWidget<ULobbyPlayerEntry>(this, PlayerEntryClass);
	if (!IsValid(playerEntry)) return;

	playerEntry->SetUsername(PlayerInfo.Username);
	ScrollBox_PlayerEntryList->AddChild(playerEntry);
}

void ULobbyPlayerEntryList::OnPlayerRemoved(const FLobbyPlayerInfo& PlayerInfo)
{
	if (ULobbyPlayerEntry* playerEntry = FindPlayerEntry(PlayerInfo.Username))
	{
		ScrollBox_PlayerEntryList->RemoveChild(playerEntry);
	}
}

