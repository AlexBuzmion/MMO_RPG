// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "GameSessionsManager.generated.h"

class AMMO_GamePlayerController;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPortalSessionReady, const FString&, IpAndPort, const FString&, OptionsParam);

UCLASS()
class DEDICATEDSERVERS_API UGameSessionsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMsg JoinGameSessionDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "GameLift")
	FOnPortalSessionReady OnPortalSessionReadyDelegate;


	
	void JoinGameSession();
	void TravelToMap(const FString& MapName, int32 RequestType);


	UFUNCTION(BlueprintCallable)
	void HandlePortalTravel(const FString& MapName, AMMO_GamePlayerController* PlayerController);
private:
	void JoinGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void HandleGameSessionStatus(const FString& Status, const FString& GameSessionID);
	void TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionID);
	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	// FString GetUniquePlayerID() const;
	
	// store the requesting player for portal travel
	TWeakObjectPtr<AMMO_GamePlayerController> PortalRequestingPlayer;
	
	// Flag indicating whether this request was initiated by a map portal.
	bool bIsPortalRequest = false;
	
	FString MapToJoin;
	FTimerHandle CreateSessionTimer;
};
