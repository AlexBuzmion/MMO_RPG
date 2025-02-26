// Alex Buzmion II 2025


#include "GameInstanceBase.h"

#include "DedicatedServers/Public/UI/GameSessions/GameSessionsManager.h"


void UGameInstanceBase::Init()
{
	Super::Init();
	if (GameSessionsManagerClass)
	{
		GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);
	}
}

void UGameInstanceBase::Shutdown()
{
	Super::Shutdown();
}

UGameSessionsManager* UGameInstanceBase::GetGameSessionsManager()
{
	return GameSessionsManager;
}
