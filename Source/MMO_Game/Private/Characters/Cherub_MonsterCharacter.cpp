// Alex Buzmion II 2025


#include "Characters/Cherub_MonsterCharacter.h"


// Sets default values
ACherub_MonsterCharacter::ACherub_MonsterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACherub_MonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACherub_MonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACherub_MonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

