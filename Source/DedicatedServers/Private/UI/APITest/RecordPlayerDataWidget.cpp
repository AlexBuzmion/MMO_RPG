// Alex Buzmion II 2025


#include "RecordPlayerDataWidget.h"

#include "Components/Button.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/CharacterSelection/CharacterSelectionManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"
#include "UI/PlayerData/PlayerDataManager.h"

void URecordPlayerDataWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CharacterSelectionManager = NewObject<UCharacterSelectionManager>(this, CharacterSelectionManagerClass);
	Button_RecordData->OnClicked.AddDynamic(this, &URecordPlayerDataWidget::RecordDataButtonClicked);
	Button_RetrieveData->OnClicked.AddDynamic(this, &URecordPlayerDataWidget::RetrieveDataButtonClicked);
}

void URecordPlayerDataWidget::RecordDataButtonClicked()
{
	check(CharacterSelectionManager);
	UDSLocalPlayerSubSystem* localPSS = CharacterSelectionManager->GetDSLocalPlayerSubSystem();
	if (!IsValid(localPSS)) return;
	
	const FString username = localPSS->Username;
	FDS_RecordPlayerDataInput input;
	input.username = username;
	input.playerData.health = 100; 
	input.playerData.level = 1;

	// CharacterSelectionManager->RecordPlayerData(input);
}

void URecordPlayerDataWidget::RetrieveDataButtonClicked()
{
	check(CharacterSelectionManager);
	UDSLocalPlayerSubSystem* localPSS = CharacterSelectionManager->GetDSLocalPlayerSubSystem();
	if (!IsValid(localPSS)) return;

	CharacterSelectionManager->RetrieveCharacters();
}
