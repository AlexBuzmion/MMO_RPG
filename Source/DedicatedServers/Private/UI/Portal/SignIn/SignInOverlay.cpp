// Alex Buzmion II 2025


#include "UI/Portal/SignIn/SignInOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "UI/API/GameSessions/JoinGameWidget.h"
#include "UI/Portal/PortalManager.h"
#include "UI/Portal/SignIn/ConfirmSignUpPage.h"
#include "UI/Portal/SignIn/SignInPage.h"
#include "UI/Portal/SignIn/SignUpPage.h"
#include "UI/Portal/SignIn/SuccessConfirmedPage.h"

void USignInOverlay::NativeConstruct() {
	Super::NativeConstruct();
	check(PortalManagerClass)
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
	check(Button_SignIn);
	check(Button_SignUp);
	check(Button_ConfirmSignUp)
	check(Button_SuccessConfirmed);

	Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	Button_ConfirmSignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowConfirmSignUpPage);
	Button_SuccessConfirmed->OnClicked.AddDynamic(this, &USignInOverlay::ShowSuccessConfirmedPage);
	
// check and bind SignIn page buttons
	check(IsValid(SignInPage));
	check(IsValid(SignInPage->Button_SignIn));
	SignInPage->Button_SignIn->OnClicked.AddDynamic(this, &USignInOverlay::SignInButtonClicked);
	check(IsValid(SignInPage->Button_Quit));
	SignInPage->Button_Quit->OnClicked.AddDynamic(PortalManager, &UPortalManager::QuitGame);
	check(IsValid(SignInPage->Button_SignUp));
	SignInPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	PortalManager->SignInStatusMsgDelegate.AddDynamic(SignInPage, &USignInPage::UpdateStatusMessage);

// check and bind SignUp page buttons	
	check(IsValid(SignUpPage));
	check(IsValid(SignUpPage->Button_Back));
	SignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
	check(IsValid(SignUpPage->Button_SignUp));
	SignUpPage->Button_SignUp->OnClicked.AddDynamic(this, &USignInOverlay::SignUpButtonClicked);
	PortalManager->SignUpStatusMsgDelegate.AddDynamic(SignUpPage, &USignUpPage::UpdateStatusMessage);
	PortalManager->OnSignUpSuccessDelegate.AddDynamic(this, &USignInOverlay::OnSignUpSucceeded);
	
// check and bind SignUp Confirmation page buttons	
	check(IsValid(ConfirmSignUpPage));
	check(IsValid(ConfirmSignUpPage->Button_Confirm));
	ConfirmSignUpPage->Button_Confirm->OnClicked.AddDynamic(this, &USignInOverlay::ConfirmButtonClicked);
	check(IsValid(ConfirmSignUpPage->Button_Back));
	ConfirmSignUpPage->Button_Back->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignUpPage);
	PortalManager->OnConfirmSignUpSuccessDelegate.AddDynamic(this, &USignInOverlay::OnConfirmSignUpSucceeded);
	PortalManager->ConfirmStatusMsgDelegate.AddDynamic(ConfirmSignUpPage, &UConfirmSignUpPage::UpdateStatusMessage);
	
// check and bind Successful Confirmation page button
	check(IsValid(SuccessConfirmedPage));
	check(IsValid(SuccessConfirmedPage->Button_Ok));
	SuccessConfirmedPage->Button_Ok->OnClicked.AddDynamic(this, &USignInOverlay::ShowSignInPage);
}

void USignInOverlay::ShowSignInPage() {
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignInPage);
}

void USignInOverlay::ShowSignUpPage() {
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SignUpPage);
}
void USignInOverlay::ShowConfirmSignUpPage() {
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(ConfirmSignUpPage);
}
void USignInOverlay::ShowSuccessConfirmedPage() {
	check(IsValid(WidgetSwitcher) && IsValid(SignInPage));
	WidgetSwitcher->SetActiveWidget(SuccessConfirmedPage);
}

void USignInOverlay::SignInButtonClicked()
{
	const FString userName = SignInPage->TextBox_UserName->GetText().ToString();
	const FString password = SignInPage->TextBox_Password->GetText().ToString();
	PortalManager->SignIn(userName, password);
}

void USignInOverlay::SignUpButtonClicked() {
	const FString userName = SignUpPage->TextBox_UserName->GetText().ToString();
	const FString password = SignUpPage->TextBox_Password->GetText().ToString();
	const FString email = SignUpPage->TextBox_Email->GetText().ToString();
	PortalManager->SignUp(userName, password, email);
	SignUpPage->Button_SignUp->SetIsEnabled(false);
}

void USignInOverlay::ConfirmButtonClicked()
{
	const FString confirmationCode = ConfirmSignUpPage->TextBox_ConfirmationCode->GetText().ToString();
	ConfirmSignUpPage->Button_Confirm->SetIsEnabled(false);
	PortalManager->SendConfirmationCode(confirmationCode);
}

void USignInOverlay::OnSignUpSucceeded()
{
	SignUpPage->ClearTextBoxes();
	ConfirmSignUpPage->TextBlock_DestinationEmail->SetText(FText::FromString(PortalManager->LastSignUpResponse.CodeDeliveryDetails.Destination));
	ShowConfirmSignUpPage();
}

void USignInOverlay::OnConfirmSignUpSucceeded()
{
	ConfirmSignUpPage->ClearTextBoxes();
	ShowSuccessConfirmedPage();
}

