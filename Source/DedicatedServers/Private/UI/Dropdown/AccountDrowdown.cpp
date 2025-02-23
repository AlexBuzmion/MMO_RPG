// Alex Buzmion II 2025


#include "UI/Dropdown/AccountDrowdown.h"

#include "Components/TextBlock.h"
#include "Player/DSLocalPlayerSubSystem.h"

void UAccountDrowdown::NativeConstruct()
{
	Super::NativeConstruct();
	APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (ULocalPlayer* localPlayer = Cast<ULocalPlayer>(localPC->Player))
	{
		if (IsValid(localPlayer)) {
			UDSLocalPlayerSubSystem* localPS = localPlayer->GetSubsystem<UDSLocalPlayerSubSystem>();
			if (IsValid(localPS))
			{
				TextBlock_ButtonText->SetText(FText::FromString(localPS->Username));
			}
		}
	}

}
