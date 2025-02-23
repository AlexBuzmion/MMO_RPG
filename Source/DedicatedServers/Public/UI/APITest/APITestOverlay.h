// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "APITestOverlay.generated.h"

class UFleetId;
class UAPITestManager;
class UListFleetBox;
struct FDS_ListFleetsResponse;

UCLASS()
class DEDICATEDSERVERS_API UAPITestOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAPITestManager> APITestManagerClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFleetId> FleetIdWidgetClass;
protected:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UListFleetBox> ListFleetsBox;
	
	UPROPERTY()
	TObjectPtr<UAPITestManager> APITestManager;
	
	UFUNCTION()
	void ListFleetsButtonClicked();

	UFUNCTION()
	void OnListFleetsResponseReceived(const FDS_ListFleetsResponse& ListFleetsResponse, bool bWasSuccessful);
};
