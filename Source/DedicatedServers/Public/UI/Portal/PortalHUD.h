// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Interfaces/HUDManagement.h"
#include "PortalHUD.generated.h"

class UDashboardOverlay;
class USignInOverlay;
class UCharacterSelectionOverlay; 

UCLASS()
class DEDICATEDSERVERS_API APortalHUD : public AHUD, public IHUDManagement
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USignInOverlay> SignInOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDashboardOverlay> DashboardOverlayClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UCharacterSelectionOverlay> CharacterSelectionOverlayClass; 
	
	// IHUDManagement Implementation
	virtual void OnSignIn() override;
	virtual void OnSignOut() override;
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<USignInOverlay> SignInOverlay;

	UPROPERTY()
	TObjectPtr<UDashboardOverlay> DashboardOverlay;

	UPROPERTY()
	TObjectPtr<UCharacterSelectionOverlay> CharacterSelectionOverlay;
};
