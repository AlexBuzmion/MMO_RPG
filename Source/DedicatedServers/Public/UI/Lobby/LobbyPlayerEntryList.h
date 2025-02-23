// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerEntryList.generated.h"

class UScrollBox;
class ULobbyPlayerEntry;
struct FLobbyPlayerInfo; 

UCLASS()
class DEDICATEDSERVERS_API ULobbyPlayerEntryList : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_PlayerEntryList;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULobbyPlayerEntry> PlayerEntryClass;
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnLobbyStateInitialized(ALobbyState* LobbyState);

	UFUNCTION()
	void CreateAndAddPlayerEntry(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION()
	void OnPlayerRemoved(const FLobbyPlayerInfo& PlayerInfo);

	UFUNCTION()
	void UpdatePlayerInfo(ALobbyState* LobbyState);

private:
	ULobbyPlayerEntry* FindPlayerEntry(const FString& Username);
};



