// Alex Buzmion II 2025

#pragma once

#include "DS_Types.generated.h"

UENUM(BlueprintType)
enum class ECountdownTimerState : uint8
{
	NotStarted UMETA(DisplayName = "Not Started"),
	Started UMETA(DisplayName = "Started"),
	Paused UMETA(DisplayName = "Paused"),
	Finished UMETA(DisplayName = "Finished"),
};

UENUM(BlueprintType)
enum class ECountdownTimerType : uint8
{
	LobbyCountdown UMETA(DisplayName = "Lobby Countdown"),
	PreMatch UMETA(DisplayName = "PreMatch"),
	InMatch UMETA(DisplayName = "InMatch"),
	PostMatch UMETA(DisplayName = "PostMatch"),
	None UMETA(DisplayName = "None"),
};

UENUM()
enum class EMatchStatus : uint8
{
	WaitingForPlayers,
	PreMatch,
	Match,
	PostMatch,
	SeamlessTravelling,
};

UENUM()
enum class ELobbyStatus : uint8
{
	WaitingForPlayers,
	CountdownToSeamlessTravel,
	SeamlessTravelling,
};

USTRUCT(BlueprintType)
struct FCountdownTimerHandle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ECountdownTimerState State = ECountdownTimerState::NotStarted;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ECountdownTimerType Type = ECountdownTimerType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CountdownTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CountdownUpdateInterval = 1;

	FTimerHandle TimerFinishedHandle{};
	FTimerDelegate TimerFinishedDelegate{};
	
	FTimerHandle TimerUpdateHandle{};
	FTimerDelegate TimerUpdateDelegate{};
};

inline bool operator==(const FCountdownTimerHandle& LHS, FCountdownTimerHandle& RHS)
{
	return LHS.Type == RHS.Type;
}