// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IPortalManagement.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "PortalManager.generated.h"

UCLASS()
class DEDICATEDSERVERS_API UPortalManager : public UHTTPRequestManager, public IIPortalManagement
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMsg SignUpStatusMsgDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMsg ConfirmStatusMsgDelegate;

	UPROPERTY(BlueprintAssignable)
	FAPIStatusMsg SignInStatusMsgDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnSignUpSuccessDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnConfirmSignUpSuccessDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnSignInSuccessDelegate;
	
	void SignIn(const FString& UserName, const FString& Password);
	void SignUp(const FString& UserName, const FString& Password, const FString& Email);
	void SendConfirmationCode(const FString& ConfirmationCode);
	void SignOut(const FString& AccessToken); 
	// IPortalManagement Implementation
	virtual void RefreshTokens(const FString& RefreshToken) override;
	UFUNCTION()
	void QuitGame();

	FDS_SignUpResponse LastSignUpResponse;
	FString LastUsername;
	
private:
	void SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void Confirmation_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,	bool bWasSuccessful);
	void SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,	bool bWasSuccessful);
};


