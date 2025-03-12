// Alex Buzmion II 2025

#include "Game/WorldMapBase_GameMode.h"

#include "GameLiftServerSDK.h"
#include "GameLiftServerSDKModels.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_AWS_GI_Subsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DS_PlayerController.h"

AWorldMapBase_GameMode::AWorldMapBase_GameMode()
{
	bUseSeamlessTravel = true;
}

void AWorldMapBase_GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode::PostLogin for %s"), *NewPlayer->GetName()); 

	if (!ExpectedMap.IsEmpty())
	{
		GetWorld()->ServerTravel(ExpectedMap + OptionsFromPreLogin, false);
		ExpectedMap = TEXT("");
	}
}

AActor* AWorldMapBase_GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	UE_LOG(LogDedicatedServers, Warning, TEXT("Choosing player start"));
	return Super::ChoosePlayerStart_Implementation(Player);
	// APlayerStart* FoundPlayerStart = nullptr;
	// UClass* PawnClass = GetDefaultPawnClassForController(Player);
	// APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>() : nullptr;
	// TArray<APlayerStart*> UnOccupiedStartPoints;
	// TArray<APlayerStart*> OccupiedStartPoints;
	// UWorld* World = GetWorld();
	//
	// //? additional local variables for the specific the games specific use case
	// FString PreviousMap; // to store the previous map the player came from
	// UDSLocalPlayerSubSystem* localPSS = nullptr; // cache the local player subsystem
	// FString loadedMap = World->GetMapName(); // cache the mapname currently loaded
	//
	// // store the previous maps name, to spawn the player to the playerstart that has a tag of the MapName it came from
	// if (UGameInstanceBase* GI = Cast<UGameInstanceBase>(World->GetGameInstance())) 
	// {
	// 	localPSS = GI->GetGameSessionsManager()->GetDSLocalPlayerSubSystem();
	// 	if (localPSS)
	// 	{
	// 		PreviousMap = localPSS->CurrentMap;
	// 	} else
	// 	{
	// 		UE_LOG(LogDedicatedServers, Warning, TEXT("Local PSS is null"));
	// 	}
	// }
	//
	// for (TActorIterator<APlayerStart> It(World); It; ++It)
	// {
	// 	APlayerStart* PlayerStart = *It;
	// 	// check if the player start's tag matches the previous map
	// 	if (PlayerStart->PlayerStartTag == PreviousMap)
	// 	{
	// 		FoundPlayerStart = PlayerStart;
	// 		break; 
	// 	}
	// 	// if no tag match, collect potential player starts
	// 	FVector ActorLocation = PlayerStart->GetActorLocation();
	// 	const FRotator ActorRotation = PlayerStart->GetActorRotation();
	// 	if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation))
	// 	{
	// 		UnOccupiedStartPoints.Add(PlayerStart);
	// 	}
	// 	else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
	// 	{
	// 		OccupiedStartPoints.Add(PlayerStart);
	// 	}
	// }
	// // fallback if no matching tag was found
	// if (FoundPlayerStart == nullptr)
	// {
	// 	if (UnOccupiedStartPoints.Num() > 0)
	// 	{
	// 		FoundPlayerStart = UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
	// 	}
	// 	else if (OccupiedStartPoints.Num() > 0)
	// 	{
	// 		FoundPlayerStart = OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
	// 	}
	// }
	// // update the local subsystem with the current map for future transitions
	// if (IsValid(localPSS))
	// {
	// 	localPSS->CurrentMap = loadedMap;
	// }
	// UE_LOG(LogDedicatedServers, Warning, TEXT("Player start chosen with Tag: %s"), *FoundPlayerStart->PlayerStartTag.ToString());
	//
	// return FoundPlayerStart;
}

AActor* AWorldMapBase_GameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}


void AWorldMapBase_GameMode::BeginPlay()
{
	Super::BeginPlay();
	InitGameLift();
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode::BeginPlay map name loaded = %s"), *GetWorld()->GetMapName()); 
}

void AWorldMapBase_GameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode::InitSeamlessTravelPlayer for %s"), *NewController->GetName()); 
}

void AWorldMapBase_GameMode::Logout(AController* Exiting)
{
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode::Logout Called")); 
	Super::Logout(Exiting);
	//NOTE: can be used to shutdown the server process if there are no players in the map or the last player is logging out
	// CheckAndStopLobbyCountdownt(); // should be start the timer to shutdown 
	RemovePlayerSession(Exiting);
}

void AWorldMapBase_GameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	
	const FString playerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString username = UGameplayStatics::ParseOption(Options, TEXT("Username"));
	const FString mapName = UGameplayStatics::ParseOption(Options, TEXT("MapName"));
	// if the mapname in the options passed is not the same as the map currently loaded 
	if ( !mapName.IsEmpty() && !GetWorld()->GetMapName().Contains(mapName))
	{
		// cache the map name for later processing in Post Login to avoid any issues in accepting and connecting.
		ExpectedMap = mapName;
		OptionsFromPreLogin = Options; 
	}
	
	TryAcceptPlayerSession(playerSessionId, username, ErrorMessage); // if this function returns anything other than an empty string for the ErrorMessage, the prelogin will fail
	
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode::PreLogin for PlayerSessionId: %s, Username: %s"), *playerSessionId, *username); 
}

FString AWorldMapBase_GameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
	const FString& Options, const FString& Portal)
{
	FString initializedString = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	const FString playerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	if (ADS_PlayerController* dSPC = Cast<ADS_PlayerController>(NewPlayerController); IsValid(dSPC))
	{
		dSPC->PlayerSessionId = playerSessionId;
		dSPC->Username = username;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::InitNewPlayer for %s"), *NewPlayerController->GetName()); 
	return initializedString;
}


void AWorldMapBase_GameMode::InitGameLift()
{
	if (UGameInstance* gameInstance = GetGameInstance(); IsValid(gameInstance))
	{
		if (AWSSubsystem = gameInstance->GetSubsystem<UDS_AWS_GI_Subsystem>(); IsValid(AWSSubsystem))
		{
			//Define the server parameters for a GameLift Anywhere fleet. These are not needed for a GameLift managed EC2 fleet.
			FServerParameters serverParameters;

			SetServerParameters(serverParameters);
			AWSSubsystem->InitGameLift(serverParameters);
		}	
	}
}

void AWorldMapBase_GameMode::SetServerParameters(FServerParameters& OutServerParameters)
{
	//AuthToken returned from the "aws gamelift get-compute-auth-token" API. Note this will expire and require a new call to the API after 15 minutes.
	if (FParse::Value(FCommandLine::Get(), TEXT("-authtoken="), OutServerParameters.m_authToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AUTH_TOKEN: %s"), *OutServerParameters.m_authToken)
	}

	if (FParse::Value(FCommandLine::Get(), TEXT("-awsregion="), OutServerParameters.m_awsRegion))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AWS_REGION: %s"), *OutServerParameters.m_awsRegion)
	}
	
	if (FParse::Value(FCommandLine::Get(), TEXT("-accesskey="), OutServerParameters.m_accessKey))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("ACCESS_KEY: %s"), *OutServerParameters.m_accessKey)
	}
	if (FParse::Value(FCommandLine::Get(), TEXT("-secretkey="), OutServerParameters.m_secretKey))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("SECRET_KEY: %s"), *OutServerParameters.m_secretKey)
	}
	if (FParse::Value(FCommandLine::Get(), TEXT("-sessiontoken="), OutServerParameters.m_sessionToken))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("SESSION_TOKEN: %s"), *OutServerParameters.m_sessionToken)
	}
	//The Host/compute-name of the GameLift Anywhere instance.
	if (FParse::Value(FCommandLine::Get(), TEXT("-hostid="), OutServerParameters.m_hostId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("HOST_ID: %s"), *OutServerParameters.m_hostId)
	}
	//The Anywhere Fleet ID.
	if (FParse::Value(FCommandLine::Get(), TEXT("-fleetid="), OutServerParameters.m_fleetId))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FLEET_ID: %s"), *OutServerParameters.m_fleetId)
	}
	//The WebSocket URL (GameLiftServiceSdkEndpoint).
	if (FParse::Value(FCommandLine::Get(), TEXT("-websocketurl="), OutServerParameters.m_webSocketUrl))
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("WEBSOCKET_URL: %s"), *OutServerParameters.m_webSocketUrl)
	}

	FString glProcessId = "";
	if (FParse::Value(FCommandLine::Get(), TEXT("-processId="), glProcessId))
	{
		OutServerParameters.m_processId = TCHAR_TO_UTF8(*glProcessId);
	}
	else
	{
		// If no ProcessId is passed as a command line argument, generate a randomized unique string.
		FString TimeString = FString::FromInt(std::time(nullptr));
		FString ProcessId = "ProcessId_" + TimeString;
		OutServerParameters.m_processId = TCHAR_TO_UTF8(*ProcessId);
	}

	//The PID of the running process
	UE_LOG(LogDedicatedServers, Log, TEXT("PID: %s"), *OutServerParameters.m_processId);
}

void AWorldMapBase_GameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username,
	FString& OutErrorMessage)
{
	if (PlayerSessionId.IsEmpty() || Username.IsEmpty())
	{
		OutErrorMessage = TEXT("PlayerSessionId or Username invalid");
		return; 
	}

#if WITH_GAMELIFT
	// using the gamelift SDK and accessing containers through the path Aws::GameLift::Server:: 
	Aws::GameLift::Server::Model::DescribePlayerSessionsRequest describePlayerSessionsRequest; // create the container for the request
	describePlayerSessionsRequest.SetPlayerSessionId(TCHAR_TO_ANSI(*PlayerSessionId)); // conversion of Char to Ansi required as it uses regular c++ array and not Unreal T Array. 
	const auto& describePlayerSessionsOutcome = Aws::GameLift::Server::DescribePlayerSessions(describePlayerSessionsRequest); // store the outcome of the request
	if (!describePlayerSessionsOutcome.IsSuccess()) // if not successful, fail and return 
	{
		OutErrorMessage = TEXT("DescribePlayerSessions Failed");
		return; 
	}

	const auto& describeplayerSessionsResult = describePlayerSessionsOutcome.GetResult(); // store the result 
	int32 count = 0;
	const Aws::GameLift::Server::Model::PlayerSession* playerSessions = describeplayerSessionsResult.GetPlayerSessions(count);
	if (playerSessions == nullptr || count == 0) // check if result is nullptr or no sessions found, fail and return 
	{
		OutErrorMessage = TEXT("GetPlayerSessions Failed");
		return;
	}

	for (int32 i = 0; i < count; i++) // iterate over all player sessions retrieved
	{
		const Aws::GameLift::Server::Model::PlayerSession& playerSession = playerSessions[i]; // store the player session locally 
		if (!Username.Equals(playerSession.GetPlayerId())) continue; // if not equal to the username, continue to iterate 
		if (playerSession.GetStatus() != Aws::GameLift::Server::Model::PlayerSessionStatus::RESERVED) // if the status of the player session is NOT reserved fail and return
		{
			OutErrorMessage = FString::Printf(TEXT("Session for %s not RESERVED; Fail PreLogin"), *Username);
			return;
		}
		const auto& acceptPlayerSessionOutcome = Aws::GameLift::Server::AcceptPlayerSession(TCHAR_TO_ANSI(*PlayerSessionId)); // if all checks are done push an accept to session command and store to outcome
		OutErrorMessage = acceptPlayerSessionOutcome.IsSuccess() ? "" : FString::Printf(TEXT("Failed to accept player session for %s"), *Username); // if not successful, update the error message to fail PreLogin()
	}
	
#endif
}