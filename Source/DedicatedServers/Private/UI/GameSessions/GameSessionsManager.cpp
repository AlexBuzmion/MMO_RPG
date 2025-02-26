// Alex Buzmion II 2025


#include "DedicatedServers/Public/UI/GameSessions/GameSessionsManager.h"

#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Data/API/APIData.h"
#include "GameInstanceBase.h"
#include "MMO_GamePlayerController.h"
#include "GameFramework/PlayerState.h"
#include "GameplayTags/DedicatedServersTags.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"

// function to initiate the request to find or create the game session with the map name through gamelifts API 
void UGameSessionsManager::TravelToMap(const FString& MapName, int32 RequestType)
{
	JoinGameSessionDelegate.Broadcast(TEXT("Loading. . ."), false);

	// set the flag for portal requests
	bIsPortalRequest = (RequestType == 1);
	
	if (MapToJoin.IsEmpty())
	{
		MapToJoin = MapName;
	}
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::JoinGameSession_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	 
	if (UGameInstanceBase* giBase = Cast<UGameInstanceBase>(GetWorld()->GetGameInstance()))
	{
		FString mapName = giBase->CurrentMapName;
	}

	TMap<FString, FString> contentParams = {
		{TEXT("mapName"), MapToJoin},
		};

	const FString content = SerializeJsonObject(contentParams);
	request->SetContentAsString(content);
	request->ProcessRequest();
}

void UGameSessionsManager::HandlePortalTravel(const FString& MapName, AMMO_GamePlayerController* PlayerController)
{
	bIsPortalRequest = true;
	MapToJoin = MapName;
	PortalRequestingPlayer = PlayerController;
	TravelToMap(MapName, 1);
}

// upon receiving the response from the FindOrCreate game session 
void UGameSessionsManager::JoinGameSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		JoinGameSessionDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		return;
	}
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			JoinGameSessionDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		}
		FDS_GameSession gameSession;
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &gameSession);

		const FString gameSessionStatus = gameSession.Status;
		const FString gameSessionId = gameSession.GameSessionId;
		HandleGameSessionStatus(gameSessionStatus, gameSessionId);
	}	
}

// if no errors were received from the response, we proceed in trying to create a player session ID 
void UGameSessionsManager::HandleGameSessionStatus(const FString& Status, const FString& GameSessionID)
{
	if (Status.Equals(TEXT("ACTIVE")))
	{
		JoinGameSessionDelegate.Broadcast(TEXT("Found active game session, Creating player session"), false);

		if (UDSLocalPlayerSubSystem* dSLocalPSS = GetDSLocalPlayerSubSystem(); IsValid(dSLocalPSS))
		{
			TryCreatePlayerSession(dSLocalPSS->Username, GameSessionID);
		}
	} else if (Status.Equals(TEXT("ACTIVATING")))
	{
		FTimerDelegate CreateSessionTimerDelegate;
		CreateSessionTimerDelegate.BindUObject(this, &ThisClass::JoinGameSession);
		// alternative to binding UObject function is to bind with a lambda
		CreateSessionTimerDelegate.BindLambda([this]
		{
			JoinGameSession(); 
		});
		APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
		if (IsValid(localPC))
		{
			localPC->GetWorldTimerManager().SetTimer(CreateSessionTimer, CreateSessionTimerDelegate, 0.5f, false);
		}
	} else
	{
		JoinGameSessionDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
	}	
}
// retry callback if the game session status is "ACTIVATING" 
void UGameSessionsManager::JoinGameSession()
{
	JoinGameSessionDelegate.Broadcast(TEXT("Loading. . ."), false);

	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::JoinGameSession_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::FindOrCreateGameSession);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	FString mapName; // character selection screen  
	if (UGameInstanceBase* giBase = Cast<UGameInstanceBase>(GetWorld()->GetGameInstance()))
	{
		mapName = giBase->CurrentMapName;
	}
	TMap<FString, FString> contentParams = {
		{TEXT("mapName"), MapToJoin},
	};
	const FString content = SerializeJsonObject(contentParams);
	request->SetContentAsString(content);
	request->ProcessRequest();
}

// function to initiate the request to create the player session ID through gamelifts API 
void UGameSessionsManager::TryCreatePlayerSession(const FString& PlayerId, const FString& GameSessionID)
{
	check(APIData)
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UGameSessionsManager::CreatePlayerSession_Response);
	const FString APIUrl = APIData->GetAPIEndpoint(DedicatedServersTags::GameSessionsAPI::CreatePlayerSession);
	request->SetURL(APIUrl);
	request->SetVerb(TEXT("POST"));
	request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	// create the body of the request by creating a json object
	TMap<FString, FString> contentParams = {
		{ TEXT("playerId"), PlayerId},
		{ TEXT("gameSessionId"), GameSessionID}
	};
	const FString content = SerializeJsonObject(contentParams);
	
	request->SetContentAsString(content); // end of constructing the body content
	request->ProcessRequest();	
}

// upon receiving the response to create player session id, and validates it does not have any errors, check if it is a map portal request or 
void UGameSessionsManager::CreatePlayerSession_Response(FHttpRequestPtr Request, FHttpResponsePtr Response,
	bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		JoinGameSessionDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
		return; 
	}
	
	TSharedPtr<FJsonObject> jsonObject;
	TSharedRef<TJsonReader<>> jsonReader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (FJsonSerializer::Deserialize(jsonReader, jsonObject))
	{
		if (ContainsErrors(jsonObject))
		{
			JoinGameSessionDelegate.Broadcast(HTTPStatusMessages::SomethingWentWrong, true);
			return; 
		}
		FDS_PlayerSession playerSession;
		FJsonObjectConverter::JsonObjectToUStruct(jsonObject.ToSharedRef(), &playerSession);

		const FString optionsParam = "?PlayerSessionId=" + playerSession.PlayerSessionId + "?Username=" + playerSession.PlayerId + "?MapName=" + MapToJoin;
		
		const FString IpAndPort = playerSession.IpAddress + TEXT(":") + FString::FromInt(playerSession.Port);
		const FName Address (*IpAndPort);

		if (bIsPortalRequest)
		{
			// For map portal requests, broadcast the connection details
			// OnPortalSessionReadyDelegate.Broadcast(IpAndPort, optionsParam);
			PortalRequestingPlayer->ClientTravelToMap(IpAndPort, optionsParam);
		}
		else
		{
			// For initial join flow, immediately open the level.
			UGameplayStatics::OpenLevel(this, Address, true, optionsParam);
		}
	}
}
		// APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
		// if (IsValid(localPC))
		// {
		// 	FInputModeGameAndUI inputModeData;
		// 	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		// 	localPC->SetInputMode(inputModeData);
		// 	localPC->SetShowMouseCursor(true);
		// }

// FString UGameSessionsManager::GetUniquePlayerID() const
// {
// 	// placeholder for player ID
// 	APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
// 	if (IsValid(localPC))
// 	{
// 		APlayerState* localPS = localPC->GetPlayerState<APlayerState>();
// 		if (IsValid(localPS) && localPS->GetUniqueId().IsValid())
// 		{
// 			return TEXT("Player_") + FString::FromInt(localPS->GetUniqueID());
// 		}
// 	}
// 	return FString();
// }
