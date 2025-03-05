// Alex Buzmion II 2025


#include "Player/CherubPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameInstanceBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/GameSessions/GameSessionsManager.h"

DEFINE_LOG_CATEGORY(LogCherubCharacter);

ACherubPlayerController::ACherubPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	bReplicates = true;
}

	
void ACherubPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION_NOTIFY(ACherubPlayerController, CachedDestination, COND_None, REPNOTIFY_Always); 
}

void ACherubPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACherubPlayerController::OnPortalInteract(const FString& TargetMap)
{
	// when player interacts with a portal, call server rpc to request travel
	ServerRequestPortalTravel(TargetMap);
}

bool ACherubPlayerController::ServerRequestPortalTravel_Validate(const FString& TargetMap)
{
	return true; 
}

void ACherubPlayerController::ServerRequestPortalTravel_Implementation(const FString& TargetMap)
{
	// get the game sessions manager from our game instance subsystem
	if (UGameInstanceBase* gameInstance = Cast<UGameInstanceBase>(GetWorld()->GetGameInstance()))
	{
		if (UGameSessionsManager* gsm = gameInstance->GetGameSessionsManager())
		{
			gsm->HandlePortalTravel(TargetMap, this);
		}
	}
}

void ACherubPlayerController::ClientTravelToMap_Implementation(const FString& IpAndPort, const FString& Options)
{
	// client calls openlevel to travel using provided ip/port and options
	UGameplayStatics::OpenLevel(this, FName(*IpAndPort), true, Options);
}

void ACherubPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ACherubPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ACherubPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ACherubPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ACherubPlayerController::OnSetDestinationReleased);

		// Setup touch input events
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ACherubPlayerController::OnInputStarted);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ACherubPlayerController::OnTouchTriggered);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ACherubPlayerController::OnTouchReleased);
		EnhancedInputComponent->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ACherubPlayerController::OnTouchReleased);
	}
	else
	{
		UE_LOG(LogCherubCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACherubPlayerController::OnInputStarted()
{
	// StopMovement();
	if (HasAuthority())
	{
		PerformStopMovement();
	} else
	{
		ServerStopMovement();
	}
}

void ACherubPlayerController::PerformStopMovement()
{
	UE_LOG(LogTemp, Warning, TEXT("Called server stop"));
	StopMovement();
}

void ACherubPlayerController::ServerStopMovement_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Requesting server to stop movement"));
	PerformStopMovement();
}

// Triggered every frame when the input is held down
void ACherubPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		// CachedDestination = Hit.Location;
		ServerSetCachedDestination(Hit.Location);
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void ACherubPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		// this should be a server RPC 
		// UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		HandleOnSetDestinationReleased(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

void ACherubPlayerController::HandleOnSetDestinationReleased(AController* Controller, const FVector& Destination)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Log, TEXT("Server moving %s to Location: %s"), *Controller->GetName(), *Destination.ToString());
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Destination);
	} else
	{
		UE_LOG(LogTemp, Log, TEXT("Client requesting to move %s to Location: %s"), *Controller->GetName(), *Destination.ToString());
		ServerSetDestination(Controller, Destination);
	}
}

void ACherubPlayerController::ServerSetDestination_Implementation(AController* Controller, const FVector& Destination)
{
	UE_LOG(LogTemp, Log, TEXT("Requesting server to Move %s to Location: %s"), *Controller->GetName(), *Destination.ToString());
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(Controller, Destination);
}

bool ACherubPlayerController::ServerSetDestination_Validate(AController* Controller, const FVector& Destination)
{
	return true; 
}

void ACherubPlayerController::ServerSetCachedDestination_Implementation(const FVector& NewDestination)
{
	CachedDestination = NewDestination;
}

// Triggered every frame when the input is held down
void ACherubPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void ACherubPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}
