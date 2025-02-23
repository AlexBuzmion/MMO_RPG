// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "JoinGameWidget.generated.h"

class UButton;
class UTextBlock;

UCLASS()
class DEDICATEDSERVERS_API UJoinGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_JoinGame; 

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_StatusMessage;

	UFUNCTION()
	void SetStatusMessage(const FString& Message, bool bShouldResetWidgets);
};
