// Alex Buzmion II 2025


#include "UI/WidgetController/Cherub_WidgetController.h"

void UCherub_WidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WidgetControllerParams)
{
	PlayerController = WidgetControllerParams.PlayerController;
	PlayerState = WidgetControllerParams.PlayerState;
	AbilitySystemComponent = WidgetControllerParams.AbilitySystemComponent;
	AttributeSet = WidgetControllerParams.AttributeSet;
}

void UCherub_WidgetController::BroadcastInitialValues()
{
}

void UCherub_WidgetController::BindCallbacksToDependencies()
{
}
