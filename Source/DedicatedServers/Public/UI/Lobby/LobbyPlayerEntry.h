// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerEntry.generated.h"

class UTextBlock;

UCLASS()
class DEDICATEDSERVERS_API ULobbyPlayerEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetUsername(const FString& Username) const;
	FString GetUsername() const;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Textblock_Username;

	
};
