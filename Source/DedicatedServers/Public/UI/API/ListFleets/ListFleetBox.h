// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ListFleetBox.generated.h"

class UScrollBox;
class UButton;

UCLASS()
class DEDICATEDSERVERS_API UListFleetBox : public UUserWidget
{
	GENERATED_BODY()

public:
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_ListFleets;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_ListFleets;
};
