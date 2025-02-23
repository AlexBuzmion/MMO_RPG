// Alex Buzmion II 2025


#include "UI/Portal/PortalManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/Portal/PortalHUD.h"

void UPortalManager::SignIn(const FString& UserName, const FString& Password)
{
	SignInStatusMsgDelegate.Broadcast(TEXT("Signing in. . ."), false);
	
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignIn_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignIn);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	LastUsername = UserName;
	
	TMap<FString, FString> contentParams = {
		{TEXT("username"), UserName},
		{TEXT("password"), Password},
	};
	const FString content = SerializeJsonObject(contentParams);
	request->SetContentAsString(content); // end of constructing the body content
	request->ProcessRequest();
}

void UPortalManager::SignIn_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		SignInStatusMsgDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true); 
	}
	
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			//todo: create a handler for when the user signup fails to show the enter 6 digit code,
			//bring back to sign in and if signed in unconfirmed, take to confirm signup screen. 
			SignInStatusMsgDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return; 
		}
		FDS_InitiateAuthResponse InitiateAuthResponse; 
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &InitiateAuthResponse);

		UDSLocalPlayerSubSystem* localPSS = GetDSLocalPlayerSubSystem();
		if (IsValid(localPSS))
		{
			localPSS->InitializeTokens(InitiateAuthResponse.AuthenticationResult, this);
			localPSS->Username = LastUsername;
			localPSS->Email = InitiateAuthResponse.Email;
		}

		APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(localPC))
		{
			
			// if (localPC->GetHUD()->GetClass()->ImplementsInterface(UHUDManagement::StaticClass())){}
			// if (localPC->GetHUD()->Implements<UHUDManagement>()) {}
			if (IHUDManagement* iHUDMgmt = Cast<IHUDManagement>(localPC->GetHUD()))
			{
				iHUDMgmt->OnSignIn();
			}
		}
	}
}

void UPortalManager::SignUp(const FString& UserName, const FString& Password, const FString& Email)
{
	SignUpStatusMsgDelegate.Broadcast(TEXT("Creating a new account"), false);
	
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignUp_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignUp);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	LastUsername = UserName;
	TMap<FString, FString> contentParams = {
		{TEXT("username"), UserName},
		{TEXT("password"), Password},
		{TEXT("email"), Email}
	};
	const FString content = SerializeJsonObject(contentParams);
	request->SetContentAsString(content); // end of constructing the body content
	request->ProcessRequest();
}

void UPortalManager::SignUp_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		SignUpStatusMsgDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true); 
	}
	
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			SignUpStatusMsgDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return; 
		}

		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &LastSignUpResponse);
		OnSignUpSuccessDelegate.Broadcast();
	}
}

void UPortalManager::SendConfirmationCode(const FString& ConfirmationCode)
{
	check(APIData)
	ConfirmStatusMsgDelegate.Broadcast(TEXT("Checking verification code. . ."), false);
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::Confirmation_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::ConfirmSignUp);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("PUT"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TMap<FString, FString> contentParams = {
		{TEXT("username"), LastUsername},
		{TEXT("confirmationCode"), ConfirmationCode}
	};
	const FString content = SerializeJsonObject(contentParams);
	request->SetContentAsString(content); // end of constructing the body content
	request->ProcessRequest();
}

void UPortalManager::Confirmation_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		ConfirmStatusMsgDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true); 
	}
	
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			if (jsonObject->HasField(TEXT("name")))
			{
				FString exception = jsonObject->GetStringField(TEXT("name"));
				if (exception.Equals("CodeMismatchException"))
				{
					ConfirmStatusMsgDelegate.Broadcast(TEXT("Incorrect verification code"), true);
					return;
				}
			}
			ConfirmStatusMsgDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return; 
		}
		OnConfirmSignUpSuccessDelegate.Broadcast();
	}
}

void UPortalManager::RefreshTokens(const FString& RefreshToken)
{
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::RefreshTokens_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignIn);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	TMap<FString, FString> contentParams = {
		{TEXT("refreshToken"), RefreshToken},
	};
	const FString content = SerializeJsonObject(contentParams); 
	request->SetContentAsString(content); // end of constructing the body content
	request->ProcessRequest();
}

void UPortalManager::RefreshTokens_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;
	
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			return; 
		}
		FDS_InitiateAuthResponse InitiateAuthResponse; 
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &InitiateAuthResponse);

		UDSLocalPlayerSubSystem* localPSS = GetDSLocalPlayerSubSystem();
		if (IsValid(localPSS))
		{
			localPSS->UpdateTokens(
				InitiateAuthResponse.AuthenticationResult.AccessToken,
				InitiateAuthResponse.AuthenticationResult.IdToken
			);
		}
	}
}

void UPortalManager::SignOut(const FString& AccessToken)
{
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UPortalManager::SignOut_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PortalAPI::SignOut);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	TMap<FString, FString> contentParams = {
		{TEXT("accessToken"), AccessToken},
	};
	const FString content = SerializeJsonObject(contentParams);
	request->SetContentAsString(content); // end of constructing the body content
	request->ProcessRequest();
}

void UPortalManager::SignOut_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	
	if (!bWasSuccessful) return;
	
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			return; 
		}
	}
	APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
	check(IsValid(localPC));
	if (IHUDManagement* iHUDMgmt = Cast<IHUDManagement>(localPC->GetHUD()))
	{
		iHUDMgmt->OnSignOut();
	}
	
}

void UPortalManager::QuitGame()
{
	APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(localPC))
	{
		UKismetSystemLibrary::QuitGame(this, localPC, EQuitPreference::Quit, false);
	}
}

