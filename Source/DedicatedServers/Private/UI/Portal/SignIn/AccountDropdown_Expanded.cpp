// Alex Buzmion II 2025


#include "AccountDropdown_Expanded.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "UI/Portal/PortalManager.h"
#include "Player/DSLocalPlayerSubSystem.h"

void UAccountDropdown_Expanded::NativeConstruct()
{
	Super::NativeConstruct();
	PortalManager = NewObject<UPortalManager>(this, PortalManagerClass);
	
	check(IsValid(Button_SignOut));
	Button_SignOut->OnClicked.AddDynamic(this, &UAccountDropdown_Expanded::Button_SignOut_Clicked);
	Button_SignOut->OnHovered.AddDynamic(this, &UAccountDropdown_Expanded::Button_SignOut_Hovered);
	Button_SignOut->OnUnhovered.AddDynamic(this, &UAccountDropdown_Expanded::Button_SignOut_Unhovered);

	check(IsValid(PortalManager));
	UDSLocalPlayerSubSystem* localPs = PortalManager->GetDSLocalPlayerSubSystem();
	check(IsValid(localPs))
	TextBlock_Email->SetText(FText::FromString(localPs->Email));
}

void UAccountDropdown_Expanded::NativePreConstruct()
{
	Super::NativePreConstruct();
	Button_SignOut_Unhovered();
	SetSignOutStyleTransparent();
}

void UAccountDropdown_Expanded::Button_SignOut_Hovered()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(TextColor_Hovered);
}

void UAccountDropdown_Expanded::Button_SignOut_Unhovered()
{
	TextBlock_SignOutButtonText->SetColorAndOpacity(TextColor_Unhovered);
}

void UAccountDropdown_Expanded::Button_SignOut_Clicked()
{
	Button_SignOut->SetIsEnabled(false);
	
	check(IsValid(PortalManager));
	UDSLocalPlayerSubSystem* localPs = PortalManager->GetDSLocalPlayerSubSystem();
	check(IsValid(localPs))
	FString accessToken = localPs->GetAccessToken();
	PortalManager->SignOut(accessToken);
	
}

void UAccountDropdown_Expanded::SetSignOutStyleTransparent()
{
	FButtonStyle style;
	FSlateBrush brush;
	brush.TintColor = FSlateColor(FLinearColor(0,0,0,0)); // make the button transparent
	style.Disabled = brush;
	style.Hovered = brush;
	style.Pressed = brush;
	style.Normal = brush;
	Button_SignOut->SetStyle(style);
}
