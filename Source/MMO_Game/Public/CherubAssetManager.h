// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "CherubAssetManager.generated.h"

UCLASS()
class MMO_GAME_API UCherubAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
		static UCherubAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
