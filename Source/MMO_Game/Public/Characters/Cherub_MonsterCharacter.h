// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "CherubCharacterBase.h"
#include "Interface/HighlightInterface.h"
#include "Cherub_MonsterCharacter.generated.h"

UCLASS()
class MMO_GAME_API ACherub_MonsterCharacter : public ACherubCharacterBase, public IHighlightInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACherub_MonsterCharacter();
	
	UFUNCTION()
	void BeginCursorOverlap(UPrimitiveComponent* TouchedComponent);
	UFUNCTION()
	void EndCursorOverlap(UPrimitiveComponent* TouchedComponent);
	// Highlighter Interface Functions
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	// Combat Interface Function/s
	virtual int32 GetActorLevel() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

};
