// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CherubCharacterBase.generated.h"

UCLASS(Abstract)
class MMO_GAME_API ACherubCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACherubCharacterBase();

protected:
	virtual void BeginPlay() override;
};
