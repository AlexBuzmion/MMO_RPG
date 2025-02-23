// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuccessConfirmedPage.generated.h"

class UButton;

UCLASS()
class DEDICATEDSERVERS_API USuccessConfirmedPage : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton>Button_Ok;
};
