// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "APITestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnListFleetsResponseReceived, const FDS_ListFleetsResponse&, ListFleetsResponse, bool, bWasSuccessful);

UCLASS()
class DEDICATEDSERVERS_API UAPITestManager : public UHTTPRequestManager
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void ListFleets();

	UPROPERTY()
	FOnListFleetsResponseReceived OnListFleetsResponseReceived;
	
	void ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
