// Alex Buzmion II 2025


#include "UI/Portal/SignIn/SignInPage.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USignInPage::UpdateStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	if (bShouldResetWidgets)
	{
		Button_SignIn->SetIsEnabled(true);
	}	
}
