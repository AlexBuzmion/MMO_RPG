// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameLiftServerSDK.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DS_AWS_GI_Subsystem.generated.h"

UCLASS()
class DEDICATEDSERVERS_API UDS_AWS_GI_Subsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UDS_AWS_GI_Subsystem();
	void InitGameLift(const FServerParameters& ServerParams);

	UPROPERTY(BlueprintReadOnly)
	bool bGameLiftInitialized;
	
private:
	void ParseCommandLineForPortNum(int32& DefaultPort);
	FProcessParameters ProcessParams;

	FString ParseCommandLineForMapName();
};
