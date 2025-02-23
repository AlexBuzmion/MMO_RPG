// Alex Buzmion II 2025


#include "UI/Dropdown/Dropdown.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"

void UDropdown::NativeConstruct() {
	Super::NativeConstruct();

	check(IsValid(Button_Expander));
	Button_Expander->OnClicked.AddDynamic(this, &UDropdown::ToggleDropdown);
	Button_Expander->OnHovered.AddDynamic(this, &UDropdown::Hovered);
	Button_Expander->OnUnhovered.AddDynamic(this, &UDropdown::Unhovered);
	
}

void UDropdown::NativePreConstruct() {
	Super::NativePreConstruct();

	SetStyleTransparent();
	Collapse();
	Unhovered();
}

void UDropdown::ToggleDropdown()
{
	if (bIsExpanded)
	{
		Collapse();
	} else
	{
		Expand();
	}
}

void UDropdown::Expand()
{
	WidgetSwitcher->SetActiveWidget(ExpandedWidget);
	bIsExpanded = true;
	Image_Dropdown->SetBrush(SlateBrush_Expand);
}

void UDropdown::Collapse()
{
	WidgetSwitcher->SetActiveWidget(CollapsedWidget);
	bIsExpanded = false;
	Image_Dropdown->SetBrush(SlateBrush_Collapse);
}

void UDropdown::Hovered()
{
	TextBlock_ButtonText->SetColorAndOpacity(TextColor_Hovered);
}

void UDropdown::Unhovered()
{
	TextBlock_ButtonText->SetColorAndOpacity(TextColor_Unhovered);
}

void UDropdown::SetStyleTransparent()
{
	FButtonStyle style;
	FSlateBrush brush;
	brush.TintColor = FSlateColor(FLinearColor(0,0,0,0)); // make the button transparent
	style.Disabled = brush;
	style.Hovered = brush;
	style.Pressed = brush;
	style.Normal = brush;
	Button_Expander->SetStyle(style);
}