// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "CherubCharacterBase.h"
#include "Cherub_PlayerCharacter.generated.h"

UCLASS()
class MMO_GAME_API ACherub_PlayerCharacter : public ACherubCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACherub_PlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
