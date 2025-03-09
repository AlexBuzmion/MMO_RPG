// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class MMO_GAME_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetActorLevel(); 
};
