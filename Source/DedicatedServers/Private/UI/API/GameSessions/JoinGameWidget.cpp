// Alex Buzmion II 2025


#include "UI/API/GameSessions/JoinGameWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void UJoinGameWidget::SetStatusMessage(const FString& Message, bool bShouldResetWidgets)
{
	Text_StatusMessage->SetText(FText::FromString(Message));
	if (bShouldResetWidgets)
	{
		Button_JoinGame->SetIsEnabled(true);
	}
}
