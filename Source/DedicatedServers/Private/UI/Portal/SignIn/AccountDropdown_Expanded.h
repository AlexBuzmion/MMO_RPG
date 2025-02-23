// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AccountDropdown_Expanded.generated.h"

class UButton;
class UTextBlock;
class UPortalManager; 

UCLASS()
class DEDICATEDSERVERS_API UAccountDropdown_Expanded : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_SignOut;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_SignOutButtonText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Email;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor TextColor_Hovered;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor TextColor_Unhovered;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPortalManager> PortalManagerClass;
protected:
	virtual void NativeConstruct() override;
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void Button_SignOut_Hovered();

	UFUNCTION()
	void Button_SignOut_Unhovered();
	
	UFUNCTION()
	void Button_SignOut_Clicked();

private:
	UPROPERTY(EditDefaultsOnly)	
	TObjectPtr<UPortalManager> PortalManager;

	void SetSignOutStyleTransparent();
};
