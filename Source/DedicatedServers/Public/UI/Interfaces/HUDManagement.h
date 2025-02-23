// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HUDManagement.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHUDManagement : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEDICATEDSERVERS_API IHUDManagement
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void OnSignIn() = 0;
	virtual void OnSignOut() = 0;
};
