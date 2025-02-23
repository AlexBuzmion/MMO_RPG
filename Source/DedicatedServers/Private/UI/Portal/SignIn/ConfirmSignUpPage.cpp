// Alex Buzmion II 2025


#include "UI/Portal/SignIn/ConfirmSignUpPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UConfirmSignUpPage::ClearTextBoxes()
{
	TextBox_ConfirmationCode->SetText(FText::GetEmpty());
	TextBlock_StatusMessage->SetText(FText::GetEmpty());
	TextBlock_DestinationEmail->SetText(FText::GetEmpty());
}

void UConfirmSignUpPage::UpdateStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	TextBox_ConfirmationCode->SetText(FText::GetEmpty());
	if (bShouldResetWidgets)
	{
		Button_Confirm->SetIsEnabled(true);
	}
}

void UConfirmSignUpPage::NativeConstruct() {
	Super::NativeConstruct();
	TextBox_ConfirmationCode->OnTextChanged.AddDynamic(this, &UConfirmSignUpPage::UpdateConfirmButtonState);
	Button_Confirm->SetIsEnabled(false);
}

void UConfirmSignUpPage::UpdateConfirmButtonState(const FText& Text)
{
	const FRegexPattern SixDigitsPattern(TEXT(R"(^\d{6}$)"));
	FRegexMatcher sixDigitMatcher(SixDigitsPattern, Text.ToString());

	const bool bValidConfirmationCode = sixDigitMatcher.FindNext();

	if (bValidConfirmationCode)
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("")));
	} else
	{
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Please enter 6 digits.")));
	}
	Button_Confirm->SetIsEnabled(bValidConfirmationCode);
}
