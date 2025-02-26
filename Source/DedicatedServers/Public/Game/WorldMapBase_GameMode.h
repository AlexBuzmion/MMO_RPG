// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "DS_GameMode_Base.h"
#include "WorldMapBase_GameMode.generated.h"

struct FServerParameters;
class UDS_AWS_GI_Subsystem;
UCLASS()
class DEDICATEDSERVERS_API AWorldMapBase_GameMode : public ADS_GameMode_Base
{
	GENERATED_BODY()
	
public:
	AWorldMapBase_GameMode();
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void BeginPlay() override;
	virtual void InitSeamlessTravelPlayer(AController* NewController) override;
	virtual void Logout(AController* Exiting) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DestinationMap; 
	
private:
	UPROPERTY()
	TObjectPtr<UDS_AWS_GI_Subsystem> AWSSubsystem;
	FString ExpectedMap;
	FString OptionsFromPreLogin; 
	void InitGameLift();
	void SetServerParameters(FServerParameters& OutServerParameters);
	void TryAcceptPlayerSession(const FString& PlayerSessionId, const FString& Username, FString& OutErrorMessage);
};
