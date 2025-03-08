// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "CherubCharacterBase.h"
#include "Cherub_PlayerCharacter.generated.h"

UCLASS(Blueprintable)
class MMO_GAME_API ACherub_PlayerCharacter : public ACherubCharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACherub_PlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	virtual void InitAbilityActorInfo() override;

	
};
