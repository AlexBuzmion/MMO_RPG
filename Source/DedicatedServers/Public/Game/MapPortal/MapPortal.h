// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapPortal.generated.h"

class UGameSessionsManager;

UCLASS()
class DEDICATEDSERVERS_API AMapPortal : public AActor
{
	GENERATED_BODY()

public:
	AMapPortal();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameSessionsManager> GameSessionsManagerClass;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Portal")
	FString DestinationMap;
	
	UFUNCTION()
	void OnPlayerEnterPortal(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							 bool bFromSweep, const FHitResult& SweepResult);

private: 
	UPROPERTY()
	TObjectPtr<UGameSessionsManager> GameSessionsManager;
};
