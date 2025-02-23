// Alex Buzmion II 2025

#include "Game/DS_LobbyGameMode.h"

#include "GameInstanceBase.h"
#include "Game/DS_AWS_GI_Subsystem.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Game/DS_GameState.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyState.h"
#include "Player/DS_PlayerController.h"

ADS_LobbyGameMode::ADS_LobbyGameMode()
{
	bUseSeamlessTravel = true;
	LobbyStatus = ELobbyStatus::WaitingForPlayers; //NOTE: the state to catch if a player is seamlessly travelling to the map before shutting down
	MinPlayers = 1; //NOTE: can be used to shutdown the server process if there are no players in the map 
	LobbyCountdownTimer.Type = ECountdownTimerType::LobbyCountdown; //NOTE:  countdown to shutdown the server as soon as the last player leaves the map/gamesession
	
}

void ADS_LobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//NOTE: this can be moved to PostLogout to start the countdown when the last player leave the map/session
	CheckAndStartLobbyCountdown();
	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::PostLogin for %s"), *NewPlayer->GetName()); 
}

void ADS_LobbyGameMode::InitSeamlessTravelPlayer(AController* NewController)
{
	Super::InitSeamlessTravelPlayer(NewController);
	
	//NOTE: can be used to shutdown the server process if there are no players in the map 
	CheckAndStartLobbyCountdown();
	
	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		AddPlayerInfoToLobbyState(NewController);
	}
	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::InitSeamlessTravelPlayer for %s"), *NewController->GetName()); 
}

void ADS_LobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	//NOTE: can be used to shutdown the server process if there are no players in the map or the last player is logging out
	CheckAndStopLobbyCountdownt(); // should be start the timer to shutdown 
	RemovePlayerSession(Exiting);
	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		RemovePlayerInfoFromLobbyState(Exiting);
	}
	// UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::Logout for %s"), *Exiting->GetName()); 
}

void ADS_LobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const FString playerSessionId = UGameplayStatics::ParseOption(Options, TEXT("PlayerSessionId"));
	const FString username = UGameplayStatics::ParseOption(Options, TEXT("Username"));

	TryAcceptPlayerSession(playerSessionId, username, ErrorMessage); // if this function returns anything other than an empty string for the ErrorMessage, the prelogin will fail
	UE_LOG(LogDedicatedServers, Warning, TEXT("ADS_LobbyGameMode::PreLogin for PlayerSessionId: %s, Username: %s"), *playerSessionId, *username); 
}

FString ADS_LobbyGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId,
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

	if (LobbyStatus != ELobbyStatus::SeamlessTravelling)
	{
		AddPlayerInfoToLobbyState(NewPlayerController);
	}
	UE_LOG(LogTemp, Warning, TEXT("ADS_LobbyGameMode::InitNewPlayer for %s"), *NewPlayerController->GetName()); 
	return initializedString;
}

void ADS_LobbyGameMode::TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username,
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

void ADS_LobbyGameMode::AddPlayerInfoToLobbyState(AController* PlayerController) const
{
	ADS_PlayerController* dSPC = Cast<ADS_PlayerController>(PlayerController);
	ADS_GameState* dSGameState = GetGameState<ADS_GameState>();
	if (IsValid(dSGameState) && IsValid(dSPC) && IsValid(dSGameState->LobbyState))
	{
		FLobbyPlayerInfo playerInfo(dSPC->Username);
		dSGameState->LobbyState->AddPlayerInfo(playerInfo);
	}
}

void ADS_LobbyGameMode::RemovePlayerInfoFromLobbyState(AController* PlayerController) const 
{
	ADS_PlayerController* dSPC = Cast<ADS_PlayerController>(PlayerController);
	ADS_GameState* dSGameState = GetGameState<ADS_GameState>();
	if (IsValid(dSGameState) && IsValid(dSPC) && IsValid(dSGameState->LobbyState))
	{
		dSGameState->LobbyState->RemovePlayerInfo(dSPC->Username);
	}
}

void ADS_LobbyGameMode::CheckAndStartLobbyCountdown()
{
	if (GetNumPlayers() >= MinPlayers && LobbyStatus == ELobbyStatus::WaitingForPlayers)
	{
		LobbyStatus = ELobbyStatus::CountdownToSeamlessTravel;
		StartCountdownTimer(LobbyCountdownTimer); 
	}
}

void ADS_LobbyGameMode::CheckAndStopLobbyCountdownt()
{
	if (GetNumPlayers() -1 < MinPlayers && LobbyStatus == ELobbyStatus::CountdownToSeamlessTravel)
	{
		LobbyStatus = ELobbyStatus::WaitingForPlayers;
		StopCountdownTimer(LobbyCountdownTimer); 
	}
}

void ADS_LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	InitGameLift();
	RegisterMapNameInGameInstance();
}

void ADS_LobbyGameMode::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	Super::OnCountdownTimerFinished(Type);
	if (Type == ECountdownTimerType::LobbyCountdown)
	{
		StopCountdownTimer(LobbyCountdownTimer);
		LobbyStatus = ELobbyStatus::SeamlessTravelling;
		TrySeamlessTravel(DestinationMap);
	}
}

void ADS_LobbyGameMode::InitGameLift()
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

void ADS_LobbyGameMode::CreateGameSession()
{
	using namespace Aws::GameLift::Server::Model;
	
#if WITH_GAMELIFT
	// Ensure GameLift is initialized
	if (!AWSSubsystem->bGameLiftInitialized)
	{
		UE_LOG(LogDedicatedServers, Error, TEXT("CreateGameSession: GameLift not initialized."));
		return;
	}
	UE_LOG(LogDedicatedServers, Warning, TEXT("CreateGameSession: GameLift initialized!"));
	UE_LOG(LogTemp, Log, TEXT("CreateGameSession: Creating a game session."));

	// Define game session properties
	UGameInstanceBase* GI = Cast<UGameInstanceBase>(GetGameInstance());
	check(IsValid(GI));
	GameProperty MapNameProperty;
	MapNameProperty.SetKey("mapName");
	MapNameProperty.SetValue(TCHAR_TO_ANSI(*GI->CurrentMapName));
	
#endif
}

void ADS_LobbyGameMode::SetServerParameters(FServerParameters& OutServerParameters)
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

void ADS_LobbyGameMode::RegisterMapNameInGameInstance()
{
	UGameInstanceBase* GI = Cast<UGameInstanceBase>(GetGameInstance());
	if (GI)
	{
		UWorld* world = GetWorld();
		if (world)
		{
			// GI->CurrentMap = TSoftObjectPtr<UWorld>(world->PersistentLevel->GetOuter());
			GI->CurrentMapName = world->GetMapName();

			UE_LOG(LogTemp, Log, TEXT("Stored Map Name in Game Instance: %s"), *GI->CurrentMapName);
		}
	}
}