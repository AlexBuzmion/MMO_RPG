// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "Blueprint/UserWidget.h"
#include "CharacterEntry.generated.h"

struct FDS_CharacterDetails;
class UImage;
class UTextBlock;
UCLASS()
class DEDICATEDSERVERS_API UCharacterEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemClicked(UObject* ListItemObject);
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CharacterName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CharacterClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CharacterLevel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_CharacterSprite;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_CreateNew;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_MapName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Highlighter;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Background;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Image_Locked;
	
	void SetIsSelected(bool IsSelected);
	
protected:

private:
	UPROPERTY(EditDefaultsOnly)
	FLinearColor Color_Hovered;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor Color_Unhovered;

};

