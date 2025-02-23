// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "GameSessionsManager.generated.h"

UCLASS()
class DEDICATEDSERVERS_API UGameSessionsManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FAPIStatusMsg JoinGameSessionDelegate;
	
	void JoinGameSession();
	void TravelToMap(const FString& MapName);
	FString MapToJoin;
	
private:
	void FindOrCreateGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	FString GetUniquePlayerID() const;
	void HandleGameSessionStatus(const FString& Status, const FString& GameSessionID);
	void TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionID);
	void CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	FTimerHandle CreateSessionTimer;
};
