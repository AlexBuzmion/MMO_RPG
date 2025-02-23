// Alex Buzmion II 2025


#include "UI/Portal/Dashboard/GamePage.h"

#include "Components/Button.h"
#include "UI/API/GameSessions/JoinGameWidget.h"
#include "UI/GameSessions/GameSessionsManager.h"

void UGamePage::NativeConstruct()
{
	Super::NativeConstruct();
	check(JoinGameWidget->Button_JoinGame);
	GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);
	GameSessionsManager->JoinGameSessionDelegate.AddDynamic(JoinGameWidget, &UJoinGameWidget::SetStatusMessage);
	
	JoinGameWidget->Button_JoinGame->OnClicked.AddDynamic(this, &UGamePage::JoinGameButtonClicked);
}

void UGamePage::JoinGameButtonClicked()
{
	check(JoinGameWidget->Button_JoinGame);
	JoinGameWidget->Button_JoinGame->SetIsEnabled(false);
	GameSessionsManager->MapToJoin = "GamePageMap";
	GameSessionsManager->JoinGameSession();
}
