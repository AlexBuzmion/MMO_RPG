// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// #include "Game/DS_GameMode.h"
#include "Game/DS_MatchGameMode.h"
#include "MMO_GameGameMode.generated.h"

UCLASS(minimalapi)
class AMMO_GameGameMode : public ADS_MatchGameMode
{
	GENERATED_BODY()

public:
	AMMO_GameGameMode();

protected:
	virtual void BeginPlay() override;
};



