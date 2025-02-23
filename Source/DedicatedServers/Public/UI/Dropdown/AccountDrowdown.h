// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Dropdown.h"
#include "AccountDrowdown.generated.h"

UCLASS()
class DEDICATEDSERVERS_API UAccountDrowdown : public UDropdown
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;	
};
