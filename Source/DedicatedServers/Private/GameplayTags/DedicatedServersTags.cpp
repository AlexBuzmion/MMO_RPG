// Alex Buzmion II 2025

#include "GameplayTags/DedicatedServersTags.h"

namespace DedicatedServersTags
{
	namespace GameSessionsAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ListFleets, "DedicatedServersTags.GameSessionsAPI.ListFleets", "Lists fleets resources on the GameSessions API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(FindOrCreateGameSession, "DedicatedServersTags.GameSessionsAPI.FindOrCreateGameSession", "Retrieves an active game session, creating one if none found on the GameSessions API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(CreatePlayerSession, "DedicatedServersTags.GameSessionsAPI.CreatePlayerSession", "Creates a new player session on the GameSessions API")
	}
	
	namespace PortalAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignUp, "DedicatedServersTags.PortalAPI.SignUp", "creates a new user in the portal API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ConfirmSignUp, "DedicatedServersTags.PortalAPI.ConfirmSignUp", "confirms a new user in the portal API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignIn, "DedicatedServersTags.PortalAPI.SignIn", "Retrieves a users Access, Id and Refresh Tokens in the portal API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SignOut, "DedicatedServersTags.PortalAPI.SignOut", "Signs out a user access in the portal API")
	}
	namespace PlayerDataAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RecordPlayerData, "DedicatedServersTags.PlayerDataAPI.RecordPlayerData", "Records player data in the portal API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RetrievePlayerData, "DedicatedServersTags.PlayerDataAPI.RetrievePlayerData", "Retrieves player data in the portal API")
	}
	namespace CharacterSelectionAPI
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(RetrieveCharacters, "DedicatedServersTags.CharacterSelectionAPI.RetrieveCharacters", "Retrieves Characters data in the portal API")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(SaveCreatedCharacter, "DedicatedServersTags.CharacterSelectionAPI.SaveCreatedCharacter", "Saves the newly created Character data in the portal API")
	}
}