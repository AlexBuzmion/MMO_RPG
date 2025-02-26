// Alex Buzmion II 2025


#include "UI/CharacterSelection/CharacterSelectionOverlay.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/CharacterSelection/CharacterCreationPage.h"
#include "UI/CharacterSelection/CharacterDataObject.h"
#include "UI/CharacterSelection/CharacterListPage.h"
#include "UI/CharacterSelection/CharacterSelectionManager.h"
#include "UI/GameSessions/GameSessionsManager.h"

void UCharacterSelectionOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	if (CharacterSelectionManagerClass)
	{
		if (!CharacterSelectionManager)
		{
		CharacterSelectionManager = NewObject<UCharacterSelectionManager>(this, CharacterSelectionManagerClass);
		}
	}
	check(IsValid(CharacterSelectionManager))
	CharacterSelectionManager->OnCharactersRetrieved.AddDynamic(this, &UCharacterSelectionOverlay::OnShowCharacterSelectionPage);
	CharacterSelectionManager->OnSaveCreatedCharacter.AddDynamic(this, &UCharacterSelectionOverlay::OnSaveCharacterCreated);
	CharacterSelectionManager->RetrieveCharacters();
	if (GameSessionsManagerClass)
	{
		GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);
	}
	check(IsValid(CharacterListPage->ListView_CharacterEntries));
	CharacterListPage->ListView_CharacterEntries->OnItemClicked().AddUObject(this, &UCharacterSelectionOverlay::HandleSelectedItemClick);
	check(IsValid(Button_Create));
	Button_Create->OnClicked.AddDynamic(this, &UCharacterSelectionOverlay::OnCreateButtonClicked);
	check(IsValid(Button_Select));
	Button_Select->OnClicked.AddDynamic(this, &UCharacterSelectionOverlay::OnSelectButtonClicked);
	check(IsValid(Button_SignOut));
	Button_SignOut->OnClicked.AddDynamic(this, &UCharacterSelectionOverlay::OnSignOutButtonClicked);
	check(IsValid(CharacterCreationPage->Button_Create))
	CharacterCreationPage->Button_Create->OnClicked.AddDynamic(this, &UCharacterSelectionOverlay::OnCreationPageCreateButtonClicked);
	check(IsValid(Button_Back));
	Button_Back->OnClicked.AddDynamic(this, &UCharacterSelectionOverlay::OnBackButtonClicked);
}

void UCharacterSelectionOverlay::ShowCharacterEntryListPage()
{
	WidgetSwitcher->SetActiveWidget(CharacterListPage);
	
}

void UCharacterSelectionOverlay::OnShowCharacterSelectionPage(FDS_GetCharactersResponse& CharacterList)
{
	CharacterListPage->UpdateCharacterEntries(CharacterList);
	ShowCharacterEntryListPage();
	if (CharacterList.characters.Num() < 1)
	{
		Button_Create->SetVisibility(ESlateVisibility::Visible);
		Button_Select->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	Button_Create->SetVisibility(ESlateVisibility::Hidden);
	Button_Select->SetVisibility(ESlateVisibility::Visible);
	Button_SignOut->SetVisibility(ESlateVisibility::Visible);
}

void UCharacterSelectionOverlay::ShowCharacterCreationPage()
{
	WidgetSwitcher->SetActiveWidget(CharacterCreationPage);
	Button_Create->SetVisibility(ESlateVisibility::Hidden);
	Button_SignOut->SetVisibility(ESlateVisibility::Hidden);
	Button_Select->SetVisibility(ESlateVisibility::Hidden);
	Button_Back->SetVisibility(ESlateVisibility::Visible);
}

void UCharacterSelectionOverlay::OnSelectButtonClicked()
{
	const UCharacterDataObject* selectedChar = CharacterListPage->ListView_CharacterEntries->GetSelectedItem<UCharacterDataObject>();
	if (selectedChar)
	{
		UE_LOG(LogTemp, Warning, TEXT("loading data for%s"), *selectedChar->CharacterName);
		// GameSessionsManager->MapToJoin = selectedChar->LastMapLocation;
		GameSessionsManager->TravelToMap(selectedChar->LastMapLocation, 0); // set to 0 to signify it is NOT a portal map request
	} else
	{
		TextBlock_StatusMessage->SetText(FText::FromString("Please select a character. . ."));
	}
}

void UCharacterSelectionOverlay::OnCreateButtonClicked()
{
	ShowCharacterCreationPage();
}

void UCharacterSelectionOverlay::OnSignOutButtonClicked()
{
	APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (IsValid(localPC))
	{
		UKismetSystemLibrary::QuitGame(this, localPC, EQuitPreference::Quit, false);
	}
}

void UCharacterSelectionOverlay::OnBackButtonClicked()
{
	ShowCharacterEntryListPage();
	if (UCharacterDataObject* charSlot1 =  Cast<UCharacterDataObject>(CharacterListPage->ListView_CharacterEntries->GetItemAt(0)))
	{
		if (charSlot1->CharacterName == "-")
		{
			UE_LOG(LogTemp, Warning, TEXT("No characters, show create button"));
			Button_Create->SetVisibility(ESlateVisibility::Visible);
		}
	}
	Button_Back->SetVisibility(ESlateVisibility::Hidden);
}

void UCharacterSelectionOverlay::HandleSelectedItemClick(UObject* ClickedItem)
{
	UCharacterDataObject* DataObject = Cast<UCharacterDataObject>(ClickedItem);
	if (DataObject)
	{
		UE_LOG(LogTemp, Log, TEXT("Character clicked: %s"), *DataObject->CharacterName);
	}
}

void UCharacterSelectionOverlay::OnCreationPageCreateButtonClicked()
{
	FString charName = CharacterCreationPage->EditableTextBox_Name->GetText().ToString();
	if (charName.IsEmpty() || charName.Len() >= 5)
	{
		CharacterSelectionManager->SaveCreatedCharacter(charName);
		UE_LOG(LogTemp, Warning, TEXT("OnCreationPageCreateButtonClicked: Initiated saving character"));
		TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Saving character. . .")));
		return; 
	}
	TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Invalid Name")));
}


void UCharacterSelectionOverlay::OnSaveCharacterCreated()
{
	TextBlock_StatusMessage->SetText(FText::FromString(TEXT("Character saved!")));
	Button_Back->SetVisibility(ESlateVisibility::Hidden);
	Button_SignOut->SetVisibility(ESlateVisibility::Visible);
	CharacterSelectionManager->RetrieveCharacters();
}