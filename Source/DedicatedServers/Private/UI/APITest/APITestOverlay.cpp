// Alex Buzmion II 2025


#include "UI/APITest/APITestOverlay.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "UI/API/ListFleets/FleetId.h"
#include "UI/API/ListFleets/ListFleetBox.h"
#include "UI/APITest/APITestManager.h"
#include "UI/HTTP/HTTPRequestTypes.h"

void UAPITestOverlay::NativeConstruct() {
	Super::NativeConstruct();
	// ensure class is assigned in blueprints
	check(APITestManagerClass);
	// create the object
	APITestManager = NewObject<UAPITestManager>(this, APITestManagerClass);

	// check if fleets box is valid and the button as well
	check(ListFleetsBox);
	check(ListFleetsBox->Button_ListFleets);
	// bind the APITestManager function to the button click event of the ListFleetsBox button
	ListFleetsBox->Button_ListFleets->OnClicked.AddDynamic(this, &UAPITestOverlay::ListFleetsButtonClicked);
}

void UAPITestOverlay::ListFleetsButtonClicked()
{
	check(APITestManager);
	APITestManager->OnListFleetsResponseReceived.AddDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);
	APITestManager->ListFleets();
	ListFleetsBox->Button_ListFleets->SetIsEnabled(false);
}

void UAPITestOverlay::OnListFleetsResponseReceived(const FDS_ListFleetsResponse& ListFleetsResponse,
	bool bWasSuccessful)
{
	if (APITestManager->OnListFleetsResponseReceived.IsAlreadyBound(this, &UAPITestOverlay::OnListFleetsResponseReceived))
	{
		APITestManager->OnListFleetsResponseReceived.RemoveDynamic(this, &UAPITestOverlay::OnListFleetsResponseReceived);
	}
	// ListFleetsBox->ScrollBox_ListFleets->ClearChildren();
	if (bWasSuccessful)
	{
		for (const FString& fleetId : ListFleetsResponse.FleetIds)
		{
			UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
			FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString(fleetId));
			ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
		}
	}
	else
	{
		UFleetId* FleetIdWidget = CreateWidget<UFleetId>(this, FleetIdWidgetClass);
		FleetIdWidget->TextBlock_FleetId->SetText(FText::FromString("Something went wrong!"));
		ListFleetsBox->ScrollBox_ListFleets->AddChild(FleetIdWidget);
	}
	ListFleetsBox->Button_ListFleets->SetIsEnabled(true);
}
