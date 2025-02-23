// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace DedicatedServersTags
{
	namespace GameSessionsAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ListFleets);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(FindOrCreateGameSession);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(CreatePlayerSession);
	}
	
	namespace PortalAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignUp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ConfirmSignUp);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignIn);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SignOut);
	}
	namespace PlayerDataAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(RecordPlayerData);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(RetrievePlayerData);
	}
	namespace CharacterSelectionAPI
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(RetrieveCharacters);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(SaveCreatedCharacter);
	}
}