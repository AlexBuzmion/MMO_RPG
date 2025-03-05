// Copyright Epic Games, Inc. All Rights Reserved.

#include "MMO_GameGameMode.h"
#include "Player/CherubPlayerController.h"
#include "UObject/ConstructorHelpers.h"

AMMO_GameGameMode::AMMO_GameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ACherubPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/Characters/Player/BP_CherubPlayerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/TopDown/Blueprints/Player/BP_CherubPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
}

void AMMO_GameGameMode::BeginPlay() {
	Super::BeginPlay();
}
