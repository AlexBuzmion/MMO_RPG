// Alex Buzmion II 2025


#include "UI/CharacterSelection/CharacterListPage.h"

#include "Components/Image.h"
#include "Components/ListView.h"
#include "UI/CharacterSelection/CharacterDataObject.h"
#include "UI/CharacterSelection/CharacterEntry.h"
#include "UI/CharacterSelection/CharacterSelectionManager.h"

void UCharacterListPage::NativeConstruct()
{
	Super::NativeConstruct();
	CharacterSelectionManager = NewObject<UCharacterSelectionManager>(this, CharacterSelectionManagerClass);
	check(IsValid(ListView_CharacterEntries));
	ListView_CharacterEntries->SetSelectionMode(ESelectionMode::Single);
	ListView_CharacterEntries->OnItemClicked().AddUObject(this, &UCharacterListPage::HandleItemClicked);
}

void UCharacterListPage::UpdateCharacterEntries(const FDS_GetCharactersResponse& CharacterList)
{
	// Clear the scroll box first to remove any existing widgets
	if (!ListView_CharacterEntries)
	{
		UE_LOG(LogTemp, Warning, TEXT("ListView_CharacterEntryList is null. Cannot update character entries."));
		return;
	}
	ListView_CharacterEntries->ClearListItems();
	
	// iterate over the array of characters
	for ( const auto& charEntry : CharacterList.characters)
	{
		const FDS_CharacterDetails& characterDetails = charEntry.Value; // Character details struct

		// Create a data object for each character
		UCharacterDataObject* dataObject = NewObject<UCharacterDataObject>(this);
		dataObject->CharacterName = characterDetails.characterName;
		dataObject->CharacterClass = characterDetails.characterClass;
		dataObject->CharacterLevel = characterDetails.level;
		dataObject->LastMapLocation = characterDetails.lastMapLocation;
		ListView_CharacterEntries->AddItem(dataObject);
		
	}
	// Add placeholders if fewer than 3 entries
	int32 NumPlaceholdersNeeded = 3 - CharacterList.characters.Num();
	for (int32 i = 0; i < NumPlaceholdersNeeded; i++)
	{
		UCharacterDataObject* placeholderData = NewObject<UCharacterDataObject>(this);
		if (ListView_CharacterEntries->GetListItems().Num() == 0)
		{
			placeholderData->CharacterName = TEXT("-");
		} else
		{
		placeholderData->CharacterName = TEXT("Locked");
		}
		placeholderData->CharacterClass = TEXT("-");
		placeholderData->CharacterLevel = 0;
		placeholderData->LastMapLocation = TEXT("");

		ListView_CharacterEntries->AddItem(placeholderData);
	}

}

void UCharacterListPage::HandleItemClicked(UObject* SelectedItem) const
{
	if (!SelectedItem) return;

	// Highlight the selected item in the list view and unhighlight the unselected 
	for (UObject* Item : ListView_CharacterEntries->GetListItems())
	{
		UCharacterEntry* EntryWidget = Cast<UCharacterEntry>(ListView_CharacterEntries->GetEntryWidgetFromItem(Item));
		if (EntryWidget)
		{
			EntryWidget->SetIsSelected(Item == SelectedItem);
		}
	}
	UCharacterDataObject* characterData = Cast<UCharacterDataObject>(SelectedItem);
	if (characterData)
	{
		UE_LOG(LogTemp, Log, TEXT("Character clicked: %s"), *characterData->CharacterName);
		// Update any relevant UI or logic here
	}
	
}