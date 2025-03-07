// Alex Buzmion II 2025


#include "Characters/Cherub_MonsterCharacter.h"

#include "AbilitySystem/Cherub_AbilitySysComponentBase.h"
#include "AbilitySystem/Cherub_AttributeSet.h"
#include "MMO_Game/MMO_Game.h"


// Sets default values
ACherub_MonsterCharacter::ACherub_MonsterCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UCherub_AbilitySysComponentBase>("Ability System Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UCherub_AttributeSet>("AttributeSet");
}

void ACherub_MonsterCharacter::BeginCursorOverlap(UPrimitiveComponent* TouchedComponent)
{
	HighlightActor();
}

void ACherub_MonsterCharacter::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED); 
}

void ACherub_MonsterCharacter::EndCursorOverlap(UPrimitiveComponent* TouchedComponent)
{
	UnHighlightActor();
}

void ACherub_MonsterCharacter::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

void ACherub_MonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GetMesh());
	GetMesh()->OnBeginCursorOver.AddDynamic(this, &ACherub_MonsterCharacter::BeginCursorOverlap);
	GetMesh()->OnEndCursorOver.AddDynamic(this, &ACherub_MonsterCharacter::EndCursorOverlap);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}


