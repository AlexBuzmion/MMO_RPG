// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapPortal.generated.h"

class UGameSessionsManager;
class AMMO_GameCharacter; 
UCLASS()
class MMO_GAME_API AMapPortal : public AActor
{
	GENERATED_BODY()

public:
	AMapPortal();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FString DestinationMap;

	FString DestinationMapAddress;

	// overlap box for detecting players
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Portal")
	class UBoxComponent* OverlapBox;
	
	UFUNCTION(BluePrintCallable)
	void OnPlayerEnterPortal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult
	);
};
