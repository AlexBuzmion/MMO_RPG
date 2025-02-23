// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SignInOverlay.generated.h"

class UJoinGameWidget;
class UPortalManager;
class UWidgetSwitcher;
class USignInPage;
class USignUpPage;
class UConfirmSignUpPage;
class USuccessConfirmedPage;
class UButton;

UCLASS()
class DEDICATEDSERVERS_API USignInOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<UPortalManager> PortalManager;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignInPage> SignInPage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USignUpPage> SignUpPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UConfirmSignUpPage> ConfirmSignUpPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USuccessConfirmedPage> SuccessConfirmedPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignIn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ConfirmSignUp;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SuccessConfirmed;

	UFUNCTION()
	void ShowSignInPage();

	UFUNCTION()
	void ShowSignUpPage();

	UFUNCTION()
	void ShowConfirmSignUpPage();

	UFUNCTION()
	void ShowSuccessConfirmedPage();

	UFUNCTION()
	void SignInButtonClicked();
	
	UFUNCTION()
	void SignUpButtonClicked();

	UFUNCTION()
	void ConfirmButtonClicked();

	UFUNCTION()
	void OnSignUpSucceeded();

	UFUNCTION()
	void OnConfirmSignUpSucceeded();
};
