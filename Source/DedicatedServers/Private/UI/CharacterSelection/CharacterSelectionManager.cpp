// Alex Buzmion II 2025


#include "UI/CharacterSelection/CharacterSelectionManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "Player/DSLocalPlayerSubSystem.h"

void UCharacterSelectionManager::RetrieveCharacters()
{
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UCharacterSelectionManager::RetrieveCharacters_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::CharacterSelectionAPI::RetrieveCharacters);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	UDSLocalPlayerSubSystem* localPSS = GetDSLocalPlayerSubSystem();
	if (IsValid(localPSS))
	{
		const FString accessToken = localPSS->GetAccessToken();
		TMap<FString, FString> contentParams = {
			{TEXT("accessToken"), accessToken},
		};
		const FString content = SerializeJsonObject(contentParams);
		request->SetContentAsString(content); // end of constructing the body content
		request->ProcessRequest();
	}
}

void UCharacterSelectionManager::RetrieveCharacters_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("RetrieveCharacters_Response: HTTP request failed."));
		return; 
	}
	const FString responseContent = Response->GetContentAsString();
	// Initialize the response struct
	FDS_GetCharactersResponse getCharactersResponse;
	
	// Check if the response is an empty array
	if (responseContent == TEXT("[]"))
	{
		UE_LOG(LogTemp, Log, TEXT("RetrieveCharacters_Response: No characters found for this user."));
		// Handle the empty character list (e.g., show a message in the UI)
		OnCharactersRetrieved.Broadcast(getCharactersResponse);
		return;
	}
	
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			UE_LOG(LogTemp, Warning, TEXT("RetrieveCharacters_Response: Response contains errors."));
			return; 
		}
		
		// Parse the "body" field
		if (!jsonObject->HasField(TEXT("body")))
		{
			UE_LOG(LogTemp, Warning, TEXT("RetrieveCharacters_Response: 'body' field is missing in the response."));
			return;
		}
		
		// Parse the "body" field into FDS_GetCharactersResponse
		TSharedPtr<FJsonObject> bodyObject = jsonObject->GetObjectField(TEXT("body"));

		// Iterate over the body keys (e.g., char1, char2)
		for (const auto& CharacterPair : bodyObject->Values)
		{
			const FString& CharacterId = CharacterPair.Key;
			const TSharedPtr<FJsonObject>* CharacterDetailsObject;

			// Ensure the value is an object
			if (bodyObject->TryGetObjectField(CharacterId, CharacterDetailsObject))
			{
				FDS_CharacterDetails CharacterDetails;

				// Parse character details into the struct
				FJsonObjectConverter::JsonObjectToUStruct(
					CharacterDetailsObject->ToSharedRef(),
					&CharacterDetails,
					0,
					0
				);

				// Add the parsed character details to the map
				getCharactersResponse.characters.Add(CharacterId, CharacterDetails);
			}
		}
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &getCharactersResponse);
		OnCharactersRetrieved.Broadcast(getCharactersResponse);
		// Log parsed characters for debugging
		for (const auto& pair : getCharactersResponse.characters)
		{
			UE_LOG(LogTemp, Log, TEXT("Character ID: %s"), *pair.Key);
			UE_LOG(LogTemp, Log, TEXT("Name: %s, Class: %s, IsAlive: %d, Last Map: %s"),
				*pair.Value.characterName,
				*pair.Value.characterClass,
				pair.Value.isAlive,
				*pair.Value.lastMapLocation);
		}
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("RetrieveCharacters_Response: Failed to deserialize JSON response."));
	}
}

void UCharacterSelectionManager::SaveCreatedCharacter(const FString& Name)
{
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UCharacterSelectionManager::SaveCreatedCharacter__Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::CharacterSelectionAPI::SaveCreatedCharacter);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	UDSLocalPlayerSubSystem* localPSS = GetDSLocalPlayerSubSystem();
	if (IsValid(localPSS))
	{
		const FString accessToken = localPSS->GetAccessToken();
		TMap<FString, FString> contentParams = {
			{TEXT("accessToken"), accessToken},
			{TEXT("characterClass"), "Cherub"},
			{TEXT("characterName"), Name},
			{TEXT("isAlive"), "1"},
			{TEXT("level"), "1"},
			{TEXT("lastMapLocation"), "Map2"},
		};
		const FString content = SerializeJsonObject(contentParams);
		request->SetContentAsString(content); // end of constructing the body content
		request->ProcessRequest();
	}
}

void UCharacterSelectionManager::SaveCreatedCharacter__Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveCreatedCharacter__Response: Data successfully saved"))
		OnSaveCreatedCharacter.Broadcast();
	}
}

