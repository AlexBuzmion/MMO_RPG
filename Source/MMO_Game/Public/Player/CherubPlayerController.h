// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "CherubPlayerController.generated.h"

class UCherubInputConfig;
/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;
class UCherub_AbilitySysComponentBase;
class USplineComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogCherubCharacter, Log, All);

UCLASS()
class MMO_GAME_API ACherubPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACherubPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/** Time Threshold to know if it was a short press */
	

	/** FX Class that we will spawn when clicking */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UNiagaraSystem* FXCursor;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;
	
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationClickAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SetDestinationTouchAction;

	// called when player interacts with a portal
	void OnPortalInteract(const FString& TargetMap);

	// server rpc to request portal travel from this client
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestPortalTravel(const FString& TargetMap);
	bool ServerRequestPortalTravel_Validate(const FString& TargetMap);
	void ServerRequestPortalTravel_Implementation(const FString& TargetMap);

	// client rpc to trigger level travel on the requesting client only
	UFUNCTION(Client, Reliable)
	void ClientTravelToMap(const FString& IpAndPort, const FString& Options);
	
protected:
	/** True if the controlled character should navigate to the mouse cursor. */
	uint32 bMoveToMouseCursor : 1;

	virtual void SetupInputComponent() override;
	
	virtual void BeginPlay() override;
	void AutoRun();
	virtual void Tick(float DeltaSeconds) override;
	
	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

private:
	FVector CachedDestination;
	float FollowTime = 0.f; // For how long it has been pressed
	float ShortPressThreshold;
	bool bAutoRunning;
	bool bTargetting;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;  

	FHitResult CursorHit;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UCherubInputConfig> InputConfig; 
	bool bIsTouch; // Is it a touch device

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY()
	TObjectPtr<UCherub_AbilitySysComponentBase> CherubAbilitySystemComponent;
	UCherub_AbilitySysComponentBase* GetCASC(); 
};

