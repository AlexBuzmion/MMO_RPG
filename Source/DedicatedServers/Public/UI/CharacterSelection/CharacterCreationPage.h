// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterCreationPage.generated.h"

class UEditableTextBox;
class UButton;

UCLASS()
class DEDICATEDSERVERS_API UCharacterCreationPage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox_Name;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Create;

};
