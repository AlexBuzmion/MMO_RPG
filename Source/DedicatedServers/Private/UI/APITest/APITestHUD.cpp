// Alex Buzmion II 2025


#include "UI/APITest/APITestHUD.h"
#include "UI/APITest/APITestOverlay.h"

void AAPITestHUD::BeginPlay() {
	Super::BeginPlay();

	APlayerController* PC = GetOwningPlayerController();
	if (IsValid(PC) && APITestOverlayClass)
	{
		APITestOverlay = CreateWidget<UAPITestOverlay>(PC, APITestOverlayClass);
		APITestOverlay->AddToViewport();
	}
}
