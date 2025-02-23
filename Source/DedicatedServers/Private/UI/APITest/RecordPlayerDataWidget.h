// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RecordPlayerDataWidget.generated.h"

class UCharacterSelectionManager;
class UButton;
class UPlayerDataManager;
UCLASS()
class DEDICATEDSERVERS_API URecordPlayerDataWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_RecordData;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_RetrieveData;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectionManager> CharacterSelectionManagerClass;

private:
	UPROPERTY()
	TObjectPtr<UCharacterSelectionManager> CharacterSelectionManager;

	UFUNCTION()
	void RecordDataButtonClicked();

	UFUNCTION()
	void RetrieveDataButtonClicked();

	
};
