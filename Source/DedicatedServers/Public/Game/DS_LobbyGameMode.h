// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "DS_GameMode_Base.h"
#include "GameLiftServerSDK.h"
#include "DS_LobbyGameMode.generated.h"

class UDS_AWS_GI_Subsystem;

UCLASS()
class DEDICATEDSERVERS_API ADS_LobbyGameMode : public ADS_GameMode_Base
{
	GENERATED_BODY()
public:
	ADS_LobbyGameMode();
	void CheckAndStartLobbyCountdown();
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnCountdownTimerFinished(ECountdownTimerType Type) override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	void CheckAndStopLobbyCountdownt();
	void RegisterMapNameInGameInstance();
	virtual void Logout(AController* Exiting) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	UPROPERTY()
	ELobbyStatus LobbyStatus;

	UPROPERTY(EditDefaultsOnly)
	int32 MinPlayers;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DestinationMap; 

private:
	UPROPERTY()
	TObjectPtr<UDS_AWS_GI_Subsystem> AWSSubsystem;

	UPROPERTY(EditDefaultsOnly)
	FCountdownTimerHandle LobbyCountdownTimer;
	
	void InitGameLift();
	void CreateGameSession();
	void SetServerParameters(FServerParameters& OutServerParameters);
	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage);
	void AddPlayerInfoToLobbyState(AController* PlayerController) const;
	void RemovePlayerInfoFromLobbyState(AController* PlayerController) const;
};

