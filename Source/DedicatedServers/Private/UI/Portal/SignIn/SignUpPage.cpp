// Alex Buzmion II 2025


#include "UI/Portal/SignIn/SignUpPage.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void USignUpPage::UpdateStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets)
{
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	if (bShouldResetWidgets)
	{
		Button_SignUp->SetIsEnabled(true);
	}
}

void USignUpPage::ClearTextBoxes()
{
	TextBox_UserName->SetText(FText::GetEmpty());
	TextBox_Password->SetText(FText::GetEmpty());
	TextBox_ConfirmPassword->SetText(FText::GetEmpty());
	TextBox_Email->SetText(FText::GetEmpty());
	
}

void USignUpPage::NativeConstruct()
{
	Super::NativeConstruct();

	TextBox_UserName->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Password->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_ConfirmPassword->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	TextBox_Email->OnTextChanged.AddDynamic(this, &USignUpPage::UpdateSignUpButtonState);
	Button_SignUp->SetIsEnabled(false);
}

void USignUpPage::UpdateSignUpButtonState(const FText& Text)
{
	// Cache input values
	const FString UserName = TextBox_UserName->GetText().ToString();
	const FString Email = TextBox_Email->GetText().ToString();
	const FString Password = TextBox_Password->GetText().ToString();
	const FString ConfirmPassword = TextBox_ConfirmPassword->GetText().ToString();

	// Validation checks
	const bool bIsUserNameValid = !UserName.IsEmpty();
	const bool bIsValidEmail = IsValidEmail(Email);
	const bool bIsPasswordLongEnough = Password.Len() >= 8;
	FString StatusMessage;
	const bool bIsStrongPassword = bIsPasswordLongEnough && IsStrongPassword(Password, StatusMessage);
	const bool bArePasswordsEqual = Password == ConfirmPassword;

	// Determine status message
	if (!bIsUserNameValid)
	{
		StatusMessage = TEXT("Please enter a valid Username.");
	}
	else if (!bIsValidEmail)
	{
		StatusMessage = TEXT("Please enter a valid Email.");
	}
	else if (!bIsPasswordLongEnough)
	{
		StatusMessage = TEXT("Password should be at least 8 characters.");
	}
	else if (!bArePasswordsEqual)
	{
		StatusMessage = TEXT("Passwords are not matching.");
	}
	else if (!bIsStrongPassword)
	{
		// StatusMessage is already set by IsStrongPassword
	}
	else
	{
		StatusMessage.Empty();
	}

	// Update UI
	TextBlock_StatusMessage->SetText(FText::FromString(StatusMessage));
	Button_SignUp->SetIsEnabled(bIsUserNameValid && bIsValidEmail && bIsPasswordLongEnough && bIsStrongPassword && bArePasswordsEqual);
}

bool USignUpPage::IsValidEmail(const FString& Email)
{
	const FRegexPattern emailPattern(TEXT(R"((^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$))"));
	FRegexMatcher matcher(emailPattern, Email);
	return matcher.FindNext();
}


bool USignUpPage::IsStrongPassword(const FString& Password, FString& StatusMessage)
{
	bool hasNumber = false;
	bool hasSpecialChar = false;
	bool hasUppercase = false;
	bool hasLowercase = false;

	for (const TCHAR& Char : Password)
	{
		if (FChar::IsDigit(Char)) hasNumber = true;
		else if (FChar::IsUpper(Char)) hasUppercase = true;
		else if (FChar::IsLower(Char)) hasLowercase = true;
		else if (!FChar::IsAlnum(Char) && !FChar::IsWhitespace(Char)) hasSpecialChar = true;

		// Early exit if all conditions are satisfied
		if (hasNumber && hasSpecialChar && hasUppercase && hasLowercase)
		{
			break;
		}
	}

	// Check conditions and set status message
	TArray<FString> Issues;
	if (!hasNumber) Issues.Add(TEXT("Password must contain at least 1 number"));
	if (!hasSpecialChar) Issues.Add(TEXT("Password must contain at least one special character"));
	if (!hasUppercase) Issues.Add(TEXT("Password must contain at least one uppercase letter"));
	if (!hasLowercase) Issues.Add(TEXT("Password must contain at least one lowercase letter"));

	if (Issues.Num() > 0)
	{
		StatusMessage = FString::Join(Issues, TEXT(", "));
		return false;
	}

	return true;
}
