// Alex Buzmion II 2025


#include "UI/PlayerData/PlayerDataManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/Interfaces/HUDManagement.h"


void UPlayerDataManager::RecordPlayerData(const FDS_RecordPlayerDataInput& RecordPlayerDataInput)
{
	// if you want a json object
	// convert struct into a json object
	TSharedPtr<FJsonObject> jsonObject = MakeShared<FJsonObject>();
	FJsonObjectConverter::UStructToJsonObject(FDS_RecordPlayerDataInput::StaticStruct(), &RecordPlayerDataInput, jsonObject.ToSharedRef());

	FString jsonString;
	FJsonObjectConverter::UStructToJsonObjectString(FDS_RecordPlayerDataInput::StaticStruct(), &RecordPlayerDataInput, jsonString);

	GEngine->AddOnScreenDebugMessage(-1, 500.0f, FColor::Red, jsonString);

	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	const FString aPIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PlayerDataAPI::RecordPlayerData);
	request->OnProcessRequestComplete().BindUObject(this, &UPlayerDataManager::RecordPlayerData_Response);
	request->SetURL(aPIUrl);
	request->SetVerb("POST");
	request->SetHeader("Content-Type", "application/json");
	request->SetContentAsString(jsonString);
	request->ProcessRequest(); 
}

void UPlayerDataManager::RetrievePlayerData()
{
	UDSLocalPlayerSubSystem* localPSS = GetDSLocalPlayerSubSystem();
	if(!IsValid(localPSS)) return;
	check(APIData); 
	// const FString accessToken = localPSS->GetAccessToken();
	TSharedPtr<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	const FString aPIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::PlayerDataAPI::RetrievePlayerData);
	request->OnProcessRequestComplete().BindUObject(this, &UPlayerDataManager::RetrievePlayerData_Response);
	request->SetURL(aPIUrl);
	request->SetVerb("POST");
	request->SetHeader("Content-Type", "application/json");
	
	TMap<FString, FString> contentParams = {
		{TEXT("accessToken"), localPSS->GetAccessToken()},
	};
	const FString content = SerializeJsonObject(contentParams);
	request->SetContentAsString(content);
	request->ProcessRequest();
}

void UPlayerDataManager::RetrievePlayerData_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		OnRetrievedPlayerDataResponseReceived.Broadcast(FDS_RetrievePlayerDataResponse());
		return;
	}
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			OnRetrievedPlayerDataResponseReceived.Broadcast(FDS_RetrievePlayerDataResponse());
			return;
		}
		
		FDS_RetrievePlayerDataResponse retrievedPlayerDataResponse;
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &retrievedPlayerDataResponse);
		retrievedPlayerDataResponse.Dump();

		OnRetrievedPlayerDataResponseReceived.Broadcast(retrievedPlayerDataResponse);
	}
}

void UPlayerDataManager::RecordPlayerData_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
                                                   bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("RecordPlayerData failed."))
	}
}


