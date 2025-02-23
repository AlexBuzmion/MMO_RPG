// Alex Buzmion II 2025


#include "Game/DS_GameMode_Base.h"

#include "aws/gamelift/server/GameLiftServerAPI.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DS_PlayerController.h"

void ADS_GameMode_Base::StartCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.TimerFinishedDelegate.BindWeakLambda(this, [&]()
	{
		OnCountdownTimerFinished(CountdownTimerHandle.Type); 
	});
	// when timer is finished
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerFinishedHandle,
		CountdownTimerHandle.TimerFinishedDelegate,
		CountdownTimerHandle.CountdownTime,
		false);

	CountdownTimerHandle.TimerUpdateDelegate.BindWeakLambda(this, [&]()
	{
		UpdateCountdownTimer(CountdownTimerHandle);
	});
	// when timer interval is updated
	GetWorldTimerManager().SetTimer(
		CountdownTimerHandle.TimerUpdateHandle,
		CountdownTimerHandle.TimerUpdateDelegate,
		CountdownTimerHandle.CountdownUpdateInterval,
		true
	);
	UpdateCountdownTimer(CountdownTimerHandle);
}

void ADS_GameMode_Base::StopCountdownTimer(FCountdownTimerHandle& CountdownTimerHandle)
{
	CountdownTimerHandle.State = ECountdownTimerState::Finished;
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerFinishedHandle);
	GetWorldTimerManager().ClearTimer(CountdownTimerHandle.TimerUpdateHandle);
	if (CountdownTimerHandle.TimerFinishedDelegate.IsBound())
	{
		CountdownTimerHandle.TimerFinishedDelegate.Unbind();
	}
	if (CountdownTimerHandle.TimerUpdateDelegate.IsBound())
	{
		CountdownTimerHandle.TimerUpdateDelegate.Unbind();
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator ; ++Iterator)
	{
		ADS_PlayerController* DS_PC = Cast<ADS_PlayerController>(Iterator->Get());
		if (IsValid(DS_PC))
		{
			DS_PC->Client_TimerStopped(0.0f, CountdownTimerHandle.Type); 
		}
	}
}

void ADS_GameMode_Base::UpdateCountdownTimer(const FCountdownTimerHandle& CountdownTimerHandle)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator ; ++Iterator)
	{
		ADS_PlayerController* DS_PC = Cast<ADS_PlayerController>(Iterator->Get());
		if (IsValid(DS_PC))
		{
			const float countdownTimeLeft = CountdownTimerHandle.CountdownTime - GetWorldTimerManager().GetTimerElapsed(CountdownTimerHandle.TimerFinishedHandle);
			DS_PC->Client_TimerUpdated(countdownTimeLeft, CountdownTimerHandle.Type);
		}
	}
}

void ADS_GameMode_Base::OnCountdownTimerFinished(ECountdownTimerType Type)
{
	
}

void ADS_GameMode_Base::TrySeamlessTravel(TSoftObjectPtr<UWorld> DestinationMap)
{
	const FString mapName = DestinationMap.ToSoftObjectPath().GetAssetName();
	if (GIsEditor)
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap); 
	} else
	{
		GetWorld()->ServerTravel(mapName);
	}
}

void ADS_GameMode_Base::RemovePlayerSession(AController* Exiting)
{
	ADS_PlayerController* dSPC = Cast<ADS_PlayerController>(Exiting);
	if (!IsValid(dSPC)) return;

#if WITH_GAMELIFT
	const FString& playerSessionId = dSPC->PlayerSessionId;
	if (!playerSessionId.IsEmpty())
	{
		Aws::GameLift::Server::RemovePlayerSession(TCHAR_TO_ANSI(*playerSessionId));
	}
#endif
}


