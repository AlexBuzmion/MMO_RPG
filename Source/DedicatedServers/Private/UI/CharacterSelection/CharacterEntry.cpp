// Alex Buzmion II 2025


#include "UI/CharacterSelection/CharacterEntry.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/CharacterSelection/CharacterDataObject.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UCharacterEntry::NativeOnListItemClicked(UObject* ListItemObject)
{
	
}

void UCharacterEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	UCharacterDataObject* CharacterData = Cast<UCharacterDataObject>(ListItemObject);
	if (!CharacterData)	return;

	if (CharacterData->CharacterName == "Locked")
	{
		Image_Locked->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	if (CharacterData->CharacterName == "-")
	{
		TextBlock_CreateNew->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	
	if (TextBlock_CharacterName)
	{
		TextBlock_CharacterName->SetText(FText::FromString(CharacterData->CharacterName));
	}

	if (TextBlock_CharacterClass)
	{
		TextBlock_CharacterClass->SetText(FText::FromString(CharacterData->CharacterClass));
	}

	if (TextBlock_CharacterLevel)
	{
		TextBlock_CharacterLevel->SetText(FText::AsNumber(CharacterData->CharacterLevel));
	}
	if (TextBlock_MapName)
	{
		TextBlock_MapName->SetText(FText::FromString(CharacterData->LastMapLocation));
	}
}

void UCharacterEntry::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	Image_Background->SetColorAndOpacity(Color_Hovered);
}

void UCharacterEntry::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	Image_Background->SetColorAndOpacity(Color_Unhovered);
}

void UCharacterEntry::SetIsSelected(bool IsSelected)
{
	check(IsValid(Image_Highlighter));
	Image_Highlighter->SetVisibility(IsSelected ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
}