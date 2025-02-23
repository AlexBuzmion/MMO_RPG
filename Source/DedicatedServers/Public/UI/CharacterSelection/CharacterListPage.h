// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterListPage.generated.h"

struct FDS_GetCharactersResponse;
class UCharacterEntry;
class UCharacterSelectionManager;
class UScrollBox;
class UListView;

UCLASS()
class DEDICATEDSERVERS_API UCharacterListPage : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectionManager> CharacterSelectionManagerClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListView> ListView_CharacterEntries;

	UFUNCTION()
	void UpdateCharacterEntries(const FDS_GetCharactersResponse& CharacterList);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ScrollBox_CharacterEntryList;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterEntry> CharacterEntryClass;
private:
	UPROPERTY()
	TObjectPtr<UCharacterSelectionManager> CharacterSelectionManager;

	UFUNCTION()
	void HandleItemClicked(UObject* SelectedItem) const;
	

};
