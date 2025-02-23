// Alex Buzmion II 2025


#include "UI/Lobby/LobbyPlayerEntry.h"

#include "Components/TextBlock.h"

void ULobbyPlayerEntry::SetUsername(const FString& Username) const
{
	Textblock_Username->SetText(FText::FromString(Username));
}

FString ULobbyPlayerEntry::GetUsername() const
{
	return Textblock_Username->GetText().ToString();
}
