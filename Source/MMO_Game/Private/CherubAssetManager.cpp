// Alex Buzmion II 2025


#include "CherubAssetManager.h"
#include "CherubGameplayTags.h"

UCherubAssetManager& UCherubAssetManager::Get()
{
	check(GEngine);
	UCherubAssetManager* cherubAM = Cast<UCherubAssetManager>(GEngine->AssetManager);
	return *cherubAM;
}

void UCherubAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	FCherubGameplayTags::InitializeNativeGameplayTags(); 
}
