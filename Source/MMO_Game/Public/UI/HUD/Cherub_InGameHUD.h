// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Cherub_InGameHUD.generated.h"

class UOverlayWidgetController;
class UCherub_UserWidget;
class UAbilitySystemComponent;
class UAttributeSet; 
struct FWidgetControllerParams;

UCLASS()
class MMO_GAME_API ACherub_InGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TObjectPtr<UCherub_UserWidget> OverlayWidget;

	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* APS, UAbilitySystemComponent* ASC, UAttributeSet* AttS);
	
protected:


private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCherub_UserWidget> OverlayWidgetClass;

	UPROPERTY()
	UOverlayWidgetController* OverlayWidgetController;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;
	
};
