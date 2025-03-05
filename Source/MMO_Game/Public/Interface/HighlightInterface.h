// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "HighlightInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHighlightInterface : public UInterface
{
	GENERATED_BODY()
};

class MMO_GAME_API IHighlightInterface
{
	GENERATED_BODY()

	
public:
	virtual void HighlightActor() = 0; 
	virtual void UnHighlightActor() = 0;
};
