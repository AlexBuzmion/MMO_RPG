// Alex Buzmion II 2025


#include "UI/Widget/Cherub_UserWidget.h"

void UCherub_UserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
