// Alex Buzmion II 2025

#include "MapPortal/MapPortal.h"

#include "GameInstanceBase.h"
#include "Components/BoxComponent.h"
#include "DedicatedServers/Public/UI/GameSessions/GameSessionsManager.h"
#include "Player/CherubPlayerController.h"


// Sets default values
AMapPortal::AMapPortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// create overlap box component and set as root
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	RootComponent = OverlapBox;

	// bind overlap event
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AMapPortal::OnPlayerEnterPortal);

}

// Called when the game starts or when spawned
void AMapPortal::BeginPlay()
{
	Super::BeginPlay();
}

void AMapPortal::OnPlayerEnterPortal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ensure the overlapping actor is valid and not self
	if (OtherActor && OtherActor != this)
	{
		// check if the overlapping actor is a pawn
		if (APawn* OverlappedPawn = Cast<APawn>(OtherActor))
		{
			// get the player controller
			if (APlayerController* PC = Cast<APlayerController>(OverlappedPawn->GetController()))
			{
				// try casting to our custom controller type
				ACherubPlayerController* MyPC = Cast<ACherubPlayerController>(PC);
				if (MyPC)
				{
					// retrieve game sessions manager from the game instance or a dedicated subsystem
					if (UGameInstanceBase* gameInstance = Cast<UGameInstanceBase>(GetWorld()->GetGameInstance()))
					{
						UGameSessionsManager* SessionsManager = gameInstance->GetGameSessionsManager();
						if (SessionsManager)
						{
							// let the sessions manager handle the portal travel for this player
							SessionsManager->HandlePortalTravel(DestinationMap, MyPC);
						}
					}
				}
			}
		}
	}
}



