// Alex Buzmion II 2025


#include "Player/CherubPlayerController.h"

// #include "EnhancedInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "CherubGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "GameInstanceBase.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/SplineComponent.h"
#include "Input/CherubInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/GameSessions/GameSessionsManager.h"

DEFINE_LOG_CATEGORY(LogCherubCharacter);

ACherubPlayerController::ACherubPlayerController()
{
	bShowMouseCursor = true;
	bReplicates = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
	ShortPressThreshold = 0.4f;
	bAutoRunning = false;
	bTargetting = false;
	AutoRunAcceptanceRadius = 50.0f;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline"); 
}

	
void ACherubPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ACherubPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACherubPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	AutoRun();
}

void ACherubPlayerController::AutoRun()
{
	if (!bAutoRunning) return; 
	if (APawn* controlledPawn = GetPawn())
	{
		const FVector locationOnSpline = Spline->FindLocationClosestToWorldLocation(controlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector direction = Spline->FindDirectionClosestToWorldLocation(locationOnSpline, ESplineCoordinateSpace::World);
		controlledPawn->AddMovementInput(direction);
		const float distanceToDestination = (locationOnSpline - CachedDestination).Length();
		if (distanceToDestination < AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
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
	if (UCherubInputComponent* CherubInputComp = Cast<UCherubInputComponent>(InputComponent))
	{
		// Setup mouse input events
		// CherubInputComp->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &ACherubPlayerController::OnInputStarted);
		// CherubInputComp->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &ACherubPlayerController::OnSetDestinationTriggered);
		// CherubInputComp->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &ACherubPlayerController::OnSetDestinationReleased);
		// CherubInputComp->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &ACherubPlayerController::OnSetDestinationReleased);
		//
		// // Setup touch input events
		// CherubInputComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Started, this, &ACherubPlayerController::OnInputStarted);
		// CherubInputComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Triggered, this, &ACherubPlayerController::OnTouchTriggered);
		// CherubInputComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Completed, this, &ACherubPlayerController::OnTouchReleased);
		// CherubInputComp->BindAction(SetDestinationTouchAction, ETriggerEvent::Canceled, this, &ACherubPlayerController::OnTouchReleased);

		CherubInputComp->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	}
	else
	{
		UE_LOG(LogCherubCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
	
}

void ACherubPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FCherubGameplayTags::Get().InputTag_LeftMB))
	{
		// bTargetting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void ACherubPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	// check if we should activate ability
	if (!InputTag.MatchesTagExact(FCherubGameplayTags::Get().InputTag_LeftMB))
	{
		if (GetCASC())
		{
			GetCASC()->AbilityInputTagReleased(InputTag);
		}
		return; 
	}
	if (bTargetting)
	{
		if (GetCASC())
		{
			GetCASC()->AbilityInputTagReleased(InputTag);
		}
	}
	else // click to move behavior 
	{
		const APawn* controlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && controlledPawn)
		{
			// create the navigation path
			if (UNavigationPath* navPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, controlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& pointLoc : navPath->PathPoints)
				{
					Spline->AddSplinePoint(pointLoc, ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(), pointLoc, 8.f, 8, FColor::Yellow, false, 5.f);
				}
				if (navPath->PathPoints.Num() > 0)
				{
					CachedDestination = navPath->PathPoints[navPath->PathPoints.Num() - 1];
				}
				bAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		bTargetting = false;
	}
}

void ACherubPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	// check if we should activate ability
	if (!InputTag.MatchesTagExact(FCherubGameplayTags::Get().InputTag_LeftMB))
	{
		if (GetCASC())
		{
			GetCASC()->AbilityInputTagHeld(InputTag);
		}
		return; 
	}
	if (bTargetting)
	{
		if (GetCASC())
		{
			GetCASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else // click to move behavior 
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (GetHitResultUnderCursor(ECC_Visibility, false, CursorHit))
		{
			CachedDestination = CursorHit.Location; // or impact point since its a line 
		}
		if (APawn* controlledPawn = GetPawn())
		{
			const FVector worldDirection = (CachedDestination - controlledPawn->GetActorLocation()).GetSafeNormal();
			controlledPawn->AddMovementInput(worldDirection);
		}
	}
}

UCherub_AbilitySysComponentBase* ACherubPlayerController::GetCASC()
{
	if (CherubAbilitySystemComponent == nullptr)
	{
		 UAbilitySystemComponent* asc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>());
		CherubAbilitySystemComponent = Cast<UCherub_AbilitySysComponentBase>(asc);
	}
	return CherubAbilitySystemComponent;
}






/* 
 * template movement implementation
 */
void ACherubPlayerController::OnInputStarted()
{
	StopMovement();
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
		CachedDestination = Hit.Location;
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
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
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