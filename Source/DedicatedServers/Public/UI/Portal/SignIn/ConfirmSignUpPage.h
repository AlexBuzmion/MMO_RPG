// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ConfirmSignUpPage.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;

UCLASS()
class DEDICATEDSERVERS_API UConfirmSignUpPage : public UUserWidget
{
	GENERATED_BODY()

public: 
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> TextBox_ConfirmationCode;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton>Button_Confirm;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton>Button_Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>TextBlock_StatusMessage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock>TextBlock_DestinationEmail;

	void ClearTextBoxes();

	UFUNCTION()
	void UpdateStatusMessage(const FString& StatusMessage, bool bShouldResetWidgets);

protected:
	virtual void NativeConstruct() override;
	
private:
	UFUNCTION()
	void UpdateConfirmButtonState(const FText& Text);
};
