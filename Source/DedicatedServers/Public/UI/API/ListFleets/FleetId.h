// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FleetId.generated.h"

class UTextBlock;

UCLASS()
class DEDICATEDSERVERS_API UFleetId : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_FleetId;
};
