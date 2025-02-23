// Alex Buzmion II 2025

#include "AWSManager.h"

void UAWSManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
// #if WITH_GAMELIFT
// 	InitGameLift();
// #endif
}

void UAWSManager::Deinitialize()
{
	Super::Deinitialize();
}
