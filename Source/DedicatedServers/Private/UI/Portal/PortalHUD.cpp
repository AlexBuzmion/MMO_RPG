// Alex Buzmion II 2025


#include "UI/Portal/PortalHUD.h"
#include "UI/Portal/SignIn/SignInOverlay.h"
#include "Blueprint/UserWidget.h"
#include "UI/CharacterSelection/CharacterSelectionOverlay.h"
#include "UI/Portal/Dashboard/DashboardOverlay.h"

void APortalHUD::BeginPlay() {
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();
	if (PC)
	{
		SignInOverlay = CreateWidget<USignInOverlay>(PC, SignInOverlayClass);
		if (IsValid(SignInOverlay))
		{
			SignInOverlay->AddToViewport();
			
		}

		FInputModeGameOnly inputModeData;
		PC->SetInputMode(inputModeData);
		PC->SetShowMouseCursor(true);
	}
}

void APortalHUD::OnSignIn()
{
	if (IsValid(SignInOverlay))
	{
		SignInOverlay->RemoveFromParent();
	}
	if (APlayerController* PC = GetOwningPlayerController())
	{
		// DashboardOverlay = CreateWidget<UDashboardOverlay>(PC, DashboardOverlayClass);
		// if (IsValid(DashboardOverlay))
		// {
		// 	DashboardOverlay->AddToViewport();
		// }
		CharacterSelectionOverlay = CreateWidget<UCharacterSelectionOverlay>(PC, CharacterSelectionOverlayClass);
		if (IsValid(CharacterSelectionOverlay))
		{
			CharacterSelectionOverlay->AddToViewport();
		}
	}
}

void APortalHUD::OnSignOut()
{
	check(IsValid(CharacterSelectionOverlay))
	check(IsValid(SignInOverlayClass))

	CharacterSelectionOverlay->RemoveFromParent();
	SignInOverlay = CreateWidget<USignInOverlay>(GetOwningPlayerController(), SignInOverlayClass);
	if (IsValid(SignInOverlay))
	{
		SignInOverlay->AddToViewport();
	}
}
