// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "UI/HTTP/HTTPRequestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "CharacterSelectionManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharactersRetrieved, FDS_GetCharactersResponse&, GetCharactersResponse);

UCLASS()
class DEDICATEDSERVERS_API UCharacterSelectionManager : public UHTTPRequestManager
{
	GENERATED_BODY()
	
public:
	void RetrieveCharacters();
	void SaveCreatedCharacter(const FString& Name);
	
	UPROPERTY(BlueprintAssignable)
	FOnCharactersRetrieved OnCharactersRetrieved;

	UPROPERTY(BlueprintAssignable)
	FOnAPIRequestSucceeded OnSaveCreatedCharacter;
	
private:
	void RetrieveCharacters_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,	bool bWasSuccessful);
	void SaveCreatedCharacter__Response(FHttpRequestPtr Request, FHttpResponsePtr Response,	bool bWasSuccessful);

};
