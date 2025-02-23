// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "PlayerDataManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRetrievedPlayerDataResponseReceived, const FDS_RetrievePlayerDataResponse, RetrievePlayerDataResponse);

struct FDS_RecordPlayerDataInput;

UCLASS()
class DEDICATEDSERVERS_API UPlayerDataManager : public UHTTPRequestManager
{
	GENERATED_BODY()
	
public:
	void RecordPlayerData(const FDS_RecordPlayerDataInput& RecordPlayerDataInput);
	void RetrievePlayerData();

	UPROPERTY()
	FOnRetrievedPlayerDataResponseReceived OnRetrievedPlayerDataResponseReceived;
	
private:
	void RecordPlayerData_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,	bool bWasSuccessful);
	void RetrievePlayerData_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,	bool bWasSuccessful);
};
