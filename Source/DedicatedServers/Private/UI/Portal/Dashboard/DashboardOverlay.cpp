// Alex Buzmion II 2025


#include "UI/Portal/Dashboard/DashboardOverlay.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Portal/Dashboard/CareerPage.h"
#include "UI/Portal/Dashboard/GamePage.h"
#include "UI/Portal/Dashboard/LeaderboardPage.h"

void UDashboardOverlay::NativeConstruct() {
	Super::NativeConstruct();

	check(IsValid(Button_Game));
	check(IsValid(Button_Career));
	check(IsValid(Button_Leaderboard));

	Button_Game->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowGamePage);
	Button_Career->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowCareerPage);
	Button_Leaderboard->OnClicked.AddDynamic(this, &UDashboardOverlay::ShowLeaderboardPage);

	ShowGamePage();
}

void UDashboardOverlay::ShowGamePage()
{
	WidgetSwitcher->SetActiveWidget(GamePage);
}

void UDashboardOverlay::ShowCareerPage()
{
	WidgetSwitcher->SetActiveWidget(CareerPage);
}

void UDashboardOverlay::ShowLeaderboardPage()
{
	WidgetSwitcher->SetActiveWidget(LeaderboardPage);
}

