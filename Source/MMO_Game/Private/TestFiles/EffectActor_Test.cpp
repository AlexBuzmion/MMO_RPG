// Alex Buzmion II 2025


#include "TestFiles/EffectActor_Test.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"


AEffectActor_Test::AEffectActor_Test()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Scene Root"));
	
}

void AEffectActor_Test::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEffectActor_Test::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GEToApply)
{
	UAbilitySystemComponent* targetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor); 
	if (targetASC == nullptr) return;
 
	check(GEClass); 
	// create an effect context
	FGameplayEffectContextHandle effectContextHandle = targetASC->MakeEffectContext();
	// set the target, source, etc. 
	effectContextHandle.AddSourceObject(this); 
	
	// create the outgoing spec to pass into the actual Gameplay Effect Application
	FGameplayEffectSpecHandle effectSpecHandle =  targetASC->MakeOutgoingSpec(GEClass, ActorLevel, effectContextHandle);
	
	// apply the effect using the spec created
	targetASC->ApplyGameplayEffectSpecToSelf(*effectSpecHandle.Data.Get());
}


