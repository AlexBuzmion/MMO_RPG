// Alex Buzmion II 2025

#include "UI/APITest/APITestManager.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpResponse.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestManager::ListFleets()
{
	check(APIData)
	
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UAPITestManager::ListFleets_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::ListFleets);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("GET"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	request->ProcessRequest();
}

void UAPITestManager::ListFleets_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, "List Fleets response received");

	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject)) // Deserialize returns a bool so we can put the function inside an if statement
	{
		// original test implementation
		// if (jsonObject->HasField(TEXT("FleetIds")))
		// {
		// 	for (TSharedPtr<FJsonValue> fleet : jsonObject->GetArrayField(TEXT("FleetIds")))
		// 	{
		// 		FString fleetString = fleet->AsString();
		// 		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 5, FColor::Green, fleetString);
		// 	}
		// }

		if (ContainsErrors(jsonObject))
		{
			OnListFleetsResponseReceived.Broadcast(FDS_ListFleetsResponse(), false);
			return;
		}
		DumpMetaData(jsonObject);

		FDS_ListFleetsResponse listFleetsResponse;
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &listFleetsResponse);
		listFleetsResponse.Dump();

		OnListFleetsResponseReceived.Broadcast(listFleetsResponse, true);
	}
}
