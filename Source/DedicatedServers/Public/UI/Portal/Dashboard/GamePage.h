// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GamePage.generated.h"

class UGameSessionsManager;
class UJoinGameWidget;
UCLASS()
class DEDICATEDSERVERS_API UGamePage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UJoinGameWidget> JoinGameWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSessionsManager> GameSessionsManagerClass;
	
protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void JoinGameButtonClicked();

	UPROPERTY()
	TObjectPtr<UGameSessionsManager> GameSessionsManager;
};
