// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Types/DS_Types.h"
#include "TimerWidget.generated.h"

class ADS_PlayerController;
class UTextBlock;

UCLASS()
class DEDICATEDSERVERS_API UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECountdownTimerType TimerType = ECountdownTimerType::None;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ADS_PlayerController> OwningPlayerController;

	UFUNCTION()
	virtual void OnTimerUpdated(float CountdownTimeLeft, ECountdownTimerType Type);
	
	UFUNCTION()
	virtual void OnTimerStopped(float CountdownTimeLeft, ECountdownTimerType Type);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTimerUpdated"))
	void K2_OnTimerUpdated(float Time, ECountdownTimerType Type);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "OnTimerStopped"))
	void K2_OnTimerStopped(float Time, ECountdownTimerType Type);

	UPROPERTY(BlueprintReadOnly)
	bool bActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeNegative = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShowCentiseconds = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bHiddenWhenInactive = true;
	
	FString FormatTimeAsString(float TimeSeconds) const;
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TextBlock_Time;

	float InternalCountdown;

	void TimerStarted(float InitialTime);
	void TimerStopped();
	void UpdateCountdown(float TimeSeconds);
};
