// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceBase.generated.h"

UCLASS()
class MMO_GAME_API UGameInstanceBase : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	virtual void Init() override;
	virtual void Shutdown() override;
	
public:
	TSoftObjectPtr<UWorld> CurrentMap;
	FString CurrentMapName; 
};
