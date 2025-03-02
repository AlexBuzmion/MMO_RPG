// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "DSLocalPlayerSubSystem.generated.h"

class IIPortalManagement;

UCLASS()
class DEDICATEDSERVERS_API UDSLocalPlayerSubSystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	void InitializeTokens(const FDS_AuthenticationResult& AuthResult, TScriptInterface<IIPortalManagement> PortalManager); 
	void SetRefreshTokenTimer();
	void UpdateTokens(const FString& AccessToken, const FString& IdToken);
	FString GetAccessToken() const;
	
	FString Username;
	FString Email;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FString CurrentMap;
	
private:
	UPROPERTY()
	FDS_AuthenticationResult AuthenticationResult;

	UPROPERTY()
	TScriptInterface<IIPortalManagement> PortalManagerInterface; 

	float TokenRefreshInterval = 2700.f; // expiration is 3600, and 2700 is 75% to allow buffer
	FTimerHandle RefreshTimerHandle;
	
};
