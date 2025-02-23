// Alex Buzmion II 2025

#include "Game/MapPortal/MapPortal.h"

#include "MMO_GameCharacter.h"
#include "DedicatedServers/DedicatedServers.h"
#include "UI/GameSessions/GameSessionsManager.h"

// Sets default values
AMapPortal::AMapPortal()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (GameSessionsManagerClass)
	{
		GameSessionsManager = NewObject<UGameSessionsManager>(this, GameSessionsManagerClass);
	}
}

// Called when the game starts or when spawned
void AMapPortal::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMapPortal::OnPlayerEnterPortal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(AMMO_GameCharacter::StaticClass())) {
		// Notify TravelManager to handle the transfer
		if (GameSessionsManager) {
			UE_LOG(LogDedicatedServers, Warning, TEXT("Initiating transfer to %s"), *DestinationMap)
			GameSessionsManager->TravelToMap(DestinationMap);
		}
	}
}


