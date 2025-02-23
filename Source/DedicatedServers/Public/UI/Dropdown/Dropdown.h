// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Dropdown.generated.h"

class UTextBlock;
class UWidgetSwitcher;
class UButton;
class UImage;

UCLASS()
class DEDICATEDSERVERS_API UDropdown : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> ExpandedWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUserWidget> CollapsedWidget;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Expander;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_ButtonText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor TextColor_Hovered;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateColor TextColor_Unhovered;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Dropdown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush SlateBrush_Expand;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateBrush SlateBrush_Collapse;

protected:
	virtual void NativeConstruct() override;
	void SetStyleTransparent();
	virtual void NativePreConstruct() override;

	UFUNCTION()
	void ToggleDropdown();

	bool bIsExpanded;

	void Expand();
	void Collapse();

	UFUNCTION()
	void Hovered();

	UFUNCTION()
	void Unhovered();
};
