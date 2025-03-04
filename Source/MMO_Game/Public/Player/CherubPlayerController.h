// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CherubPlayerController.generated.h"

/** Forward declaration to improve compiling times */
class UNiagaraSystem;
class UInputMappingContext;
class UInputAction;

DECLARE_LOG_CATEGORY_EXTERN(LogCherubCharacter, Log, All);

UCLASS()
class MMO_GAME_API ACherubPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ACherubPlayerController();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	/** Time Threshold to know if it was a short press */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	float ShortPressThreshold;

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
	
	// To add mapping context
	virtual void BeginPlay() override;

	/** Input handlers for SetDestination action. */
	void OnInputStarted();
	void OnSetDestinationTriggered();
	void OnSetDestinationReleased();
	void OnTouchTriggered();
	void OnTouchReleased();

	UFUNCTION()
	void PerformStopMovement();
	UFUNCTION(Server, Reliable)
	void ServerStopMovement();
	
	UFUNCTION()
	void HandleOnSetDestinationReleased(AController* Controller, const FVector& Destination);
	
	UFUNCTION(Server, Reliable)
	void ServerSetCachedDestination(const FVector& NewDestination);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetDestination(AController* Controller, const FVector& Destination);
	bool ServerSetDestination_Validate(AController* Controller, const FVector& Destination); 

	UPROPERTY(BlueprintReadOnly, Replicated)
	FVector CachedDestination;
private:

	bool bIsTouch; // Is it a touch device
	float FollowTime; // For how long it has been pressed
};
