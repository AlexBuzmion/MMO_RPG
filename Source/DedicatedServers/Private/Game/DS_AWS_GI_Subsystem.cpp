// Alex Buzmion II 2025

#include "Game/DS_AWS_GI_Subsystem.h"
#include "DedicatedServers/DedicatedServers.h"

UDS_AWS_GI_Subsystem::UDS_AWS_GI_Subsystem()
{
	bGameLiftInitialized = false;
}

void UDS_AWS_GI_Subsystem::InitGameLift(const FServerParameters& ServerParams)
{
	if (bGameLiftInitialized) return;

#if WITH_GAMELIFT
	UE_LOG(LogDedicatedServers, Log, TEXT("Initializing the GameLift Server"));

	//Get the module first.
	FGameLiftServerSDKModule* gameLiftSdkModule = &FModuleManager::LoadModuleChecked<FGameLiftServerSDKModule>(FName("GameLiftServerSDK"));
	
	//InitSDK establishes a local connection with GameLift's agent to enable further communication.
	//Use InitSDK(serverParameters) for a GameLift Anywhere fleet. 
	//Use InitSDK() for a GameLift managed EC2 fleet.
	gameLiftSdkModule->InitSDK(ServerParams);
	/*Implement callback function OnStartGameSession
	GameLift sends a game session activation request to the game server
	and passes a game session object with game properties and other settings.
	Here is where a game server takes action based on the game session object.
	When the game server is ready to receive incoming player connections, 
	it invokes the server SDK call ActivateGameSession().*/
	auto onGameSession = [=](Aws::GameLift::Server::Model::GameSession gameSession)
	{
		FString gameSessionId = FString(gameSession.GetGameSessionId());
		UE_LOG(LogDedicatedServers, Log, TEXT("GameSession Initializing: %s"), *gameSessionId);
		gameLiftSdkModule->ActivateGameSession();
	};
	ProcessParams.OnStartGameSession.BindLambda(onGameSession);

	//Implement callback function OnProcessTerminate
	//GameLift invokes this callback before shutting down the instance hosting this game server.
	//It gives the game server a chance to save its state, communicate with services, etc., 
	//and initiate shut down. When the game server is ready to shut down, it invokes the 
	//server SDK call ProcessEnding() to tell GameLift it is shutting down.
	auto onProcessTerminate = [=]()
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Game Server Process is terminating"));
		gameLiftSdkModule->ProcessEnding();
	};
	ProcessParams.OnTerminate.BindLambda(onProcessTerminate);

	//Implement callback function OnHealthCheck
	//GameLift invokes this callback approximately every 60 seconds.
	//A game server might want to check the health of dependencies, etc.
	//Then it returns health status true if healthy, false otherwise.
	//The game server must respond within 60 seconds, or GameLift records 'false'.
	//In this example, the game server always reports healthy.
	auto onHealthCheck = []()
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("Performing Health Check"));
		return true;
	};
	ProcessParams.OnHealthCheck.BindLambda(onHealthCheck);

	// get the default port value
	int32 defaultPort = FURL::UrlConfig.DefaultPort;
	ParseCommandLineForPortNum(defaultPort);
	ProcessParams.port = defaultPort;

	UE_LOG(LogDedicatedServers, Log, TEXT("Calling ProcessReady()"));
	gameLiftSdkModule->ProcessReady(ProcessParams);
#endif
	bGameLiftInitialized = true;
}

void UDS_AWS_GI_Subsystem::ParseCommandLineForPortNum(int32& DefaultPort)
{
	TArray<FString> parsedTokens;
	TArray<FString> parsedSwitches;
	FCommandLine::Parse(FCommandLine::Get(), parsedTokens, parsedSwitches);
	for (const FString& switchIter : parsedSwitches)
	{
		FString key;
		FString value;
		if (switchIter.Split("=", &key, &value))
		{
			if (key.Equals(TEXT("port"), ESearchCase::IgnoreCase) && value.IsNumeric())
			{
				LexFromString(DefaultPort, *value);
				return;
			}
		}
	}
}

FString UDS_AWS_GI_Subsystem::ParseCommandLineForMapName()
{
	// Get the full command line
	FString FullCommandLine = FCommandLine::Get();

	// Split the command line into tokens
	TArray<FString> Tokens;
	FullCommandLine.ParseIntoArray(Tokens, TEXT(" "), true);

	// Iterate through tokens to find the map name
	for (const FString& Token : Tokens)
	{
		// Skip arguments that have "=" (these are likely key-value pairs like -authtoken=...)
		if (!Token.Contains(TEXT("=")) && !Token.StartsWith(TEXT("-")))
		{
			UE_LOG(LogDedicatedServers, Log, TEXT("Parsed Map Name: %s"), *Token);
			return Token; // Return the first standalone token as the map name
		}
	}

	// Fallback if no map name is found
	UE_LOG(LogDedicatedServers, Warning, TEXT("Failed to parse map name from command line."));
	return TEXT("Map1");
}
