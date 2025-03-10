// Alex Buzmion II 2025


#include "AbilitySystem/CherubAbilitySystemBPLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "Player/Cherub_PlayerState.h"
#include "UI/HUD/Cherub_InGameHUD.h"
#include "UI/WidgetController/Cherub_WidgetController.h"

UOverlayWidgetController* UCherubAbilitySystemBPLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACherub_InGameHUD* cHUD = Cast<ACherub_InGameHUD>(pc->GetHUD()))
		{
			ACherub_PlayerState* ps = pc->GetPlayerState<ACherub_PlayerState>();
			UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();
			UAttributeSet* attrSet = ps->GetAttributeSet();
			const FWidgetControllerParams wcParams(pc, ps, asc, attrSet);
			return cHUD->GetOverlayWidgetController(wcParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UCherubAbilitySystemBPLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* pc = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ACherub_InGameHUD* cHUD = Cast<ACherub_InGameHUD>(pc->GetHUD()))
		{
			ACherub_PlayerState* ps = pc->GetPlayerState<ACherub_PlayerState>();
			UAbilitySystemComponent* asc = ps->GetAbilitySystemComponent();
			UAttributeSet* attrSet = ps->GetAttributeSet();
			const FWidgetControllerParams wcParams(pc, ps, asc, attrSet);
			return cHUD->GetAttributeMenuWidgetController(wcParams);
		}
	}
	return nullptr;
}
