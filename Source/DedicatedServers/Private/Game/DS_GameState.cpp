// Alex Buzmion II 2025


#include "Game/DS_GameState.h"

#include "Lobby/LobbyState.h"
#include "Net/UnrealNetwork.h"

ADS_GameState::ADS_GameState()
{
	bReplicates = true;
	
}

void ADS_GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADS_GameState, LobbyState); 
}

void ADS_GameState::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CreateLobbyState();
		OnLobbyStateInitialized.Broadcast(LobbyState);
	}
}

void ADS_GameState::CreateLobbyState()
{
	if (UWorld* world = GetWorld(); IsValid(world))
	{
		FActorSpawnParameters spawnParams;
		LobbyState = world->SpawnActor<ALobbyState>(
			ALobbyState::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			spawnParams
		);
		if (IsValid(LobbyState))
		{
			LobbyState->SetOwner(this);
		}
	}
}

void ADS_GameState::OnRep_LobbyState()
{
	OnLobbyStateInitialized.Broadcast(LobbyState);
}
