// Alex Buzmion II 2025


#include "UI/HUD/Cherub_InGameHUD.h"

#include "UI/Widget/Cherub_UserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ACherub_InGameHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	// check if widget controller is null
	if (OverlayWidgetController == nullptr)
	{	// Create one if null 
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* ACherub_InGameHUD::GetAttributeMenuWidgetController(
	const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

void ACherub_InGameHUD::InitOverlay(APlayerController* PC, APlayerState* APS, UAbilitySystemComponent* ASC,
                                    UAttributeSet* AttS)
{
	//perform a check and handle unassigned Classes
	checkf(OverlayWidgetClass, TEXT("Overlay Widget class unitialized, fill BP_CherubInGameHUD"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller class unitialized, fill BP_CherubInGameHUD"))
	
	//create the user widget
	UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UCherub_UserWidget>(widget);
	//create the widget controller params 
	const FWidgetControllerParams WCParams(PC, APS, ASC, AttS);
	//create the widget controller
	UOverlayWidgetController* overlayWidgetController = GetOverlayWidgetController(WCParams);
	//set the widget controller of the overlay widget
	OverlayWidget->SetWidgetController(overlayWidgetController);
	overlayWidgetController->BroadcastInitialValues();
	
	widget->AddToViewport();
}

