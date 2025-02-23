// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionOverlay.generated.h"

class UTextBlock;
class UCharacterCreationPage;
class UCharacterListPage;
class UCharacterSelectionManager;
class UGameSessionsManager;

UCLASS()
class DEDICATEDSERVERS_API UCharacterSelectionOverlay : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
		
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectionManager> CharacterSelectionManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSessionsManager> GameSessionsManagerClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterListPage> CharacterListPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCharacterCreationPage> CharacterCreationPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Select;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Create;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignOut;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Back;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_StatusMessage;
	
protected:
	UFUNCTION()
	void OnCreationPageCreateButtonClicked();
	
	virtual void NativeConstruct() override;

private:
	UPROPERTY()
	TObjectPtr<UCharacterSelectionManager> CharacterSelectionManager;

	UPROPERTY()
	TObjectPtr<UGameSessionsManager> GameSessionsManager;

	UFUNCTION()
	void ShowCharacterEntryListPage();

	UFUNCTION()
	void OnShowCharacterSelectionPage(FDS_GetCharactersResponse& CharacterList);

	UFUNCTION()
	void ShowCharacterCreationPage();
	
	UFUNCTION()
	void OnSelectButtonClicked();

	UFUNCTION()
	void OnCreateButtonClicked();

	UFUNCTION()
	void OnSignOutButtonClicked();

	UFUNCTION()
	void OnBackButtonClicked();
	
	UFUNCTION()
	void HandleSelectedItemClick(UObject* ClickedItem);

	UFUNCTION()
	void OnSaveCharacterCreated();
};



