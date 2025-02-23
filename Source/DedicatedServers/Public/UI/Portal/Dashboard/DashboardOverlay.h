// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DashboardOverlay.generated.h"

class ULeaderboardPage;
class UCareerPage;
class UGamePage;
class UWidgetSwitcher;
class UButton;

UCLASS()
class DEDICATEDSERVERS_API UDashboardOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGamePage> GamePage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCareerPage> CareerPage;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<ULeaderboardPage> LeaderboardPage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Game;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Career;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button_Leaderboard;

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ShowGamePage();

	UFUNCTION()
	void ShowCareerPage();

	UFUNCTION()
	void ShowLeaderboardPage();
};
