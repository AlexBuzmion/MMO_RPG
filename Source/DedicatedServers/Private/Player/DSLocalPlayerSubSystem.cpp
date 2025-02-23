// Alex Buzmion II 2025


#include "Player/DSLocalPlayerSubSystem.h"

#include "UI/Portal/Interfaces/IPortalManagement.h"

void UDSLocalPlayerSubSystem::InitializeTokens(const FDS_AuthenticationResult& AuthResult, TScriptInterface<IIPortalManagement> PortalManager)
{
	AuthenticationResult = AuthResult;
	PortalManagerInterface = PortalManager;
	SetRefreshTokenTimer();
	
}

void UDSLocalPlayerSubSystem::SetRefreshTokenTimer()
{
	UWorld* world = GetWorld();
	if (IsValid(world) && IsValid(PortalManagerInterface.GetObject()))
	{
		FTimerDelegate refreshDelegate;
		refreshDelegate.BindLambda([this]() { PortalManagerInterface->RefreshTokens(AuthenticationResult.RefreshToken); });
		world->GetTimerManager().SetTimer(RefreshTimerHandle, refreshDelegate, TokenRefreshInterval, false);
	}
}

void UDSLocalPlayerSubSystem::UpdateTokens(const FString& AccessToken, const FString& IdToken)
{
	AuthenticationResult.AccessToken = AccessToken;
	AuthenticationResult.IdToken = IdToken;
	AuthenticationResult.Dump();
	SetRefreshTokenTimer();
}

FString UDSLocalPlayerSubSystem::GetAccessToken() const
{
	return AuthenticationResult.AccessToken;
}
