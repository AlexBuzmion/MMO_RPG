// Alex Buzmion II 2025


#include "Characters/Cherub_PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CherubPlayerController.h"
#include "Player/Cherub_PlayerState.h"
#include "UI/HUD/Cherub_InGameHUD.h"


// Sets default values
ACherub_PlayerCharacter::ACherub_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

// Called when the game starts or when spawned
void ACherub_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ACherub_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	// this initializes the ability actor info for the server
	InitAbilityActorInfo();
}

void ACherub_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	// this initializes the ability actor info for the client
	InitAbilityActorInfo();
}

void ACherub_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACherub_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACherub_PlayerCharacter::InitAbilityActorInfo()
{
	ACherub_PlayerState* cPS = GetPlayerState<ACherub_PlayerState>();
	if (cPS)
	{
		cPS->GetAbilitySystemComponent()->InitAbilityActorInfo(cPS, this);
		Cast<UCherub_AbilitySysComponentBase>(cPS->GetAbilitySystemComponent())->AbilityActorInfoSet();
		AbilitySystemComponent = cPS->GetAbilitySystemComponent();
		AttributeSet = cPS->GetAttributeSet();
		// non-assert check to continue only if this runs on the local player controller
		if (ACherubPlayerController* cPC = Cast<ACherubPlayerController>(GetController()))
		{	// get and cast HUD to our custom HUD 
			if (ACherub_InGameHUD* cHUD = Cast<ACherub_InGameHUD>(cPC->GetHUD()))
			{	// initialize the overlay
				cHUD->InitOverlay(cPC, cPS, AbilitySystemComponent, AttributeSet);
			}
		}
		InitializePrimaryAttributes();
	}

}