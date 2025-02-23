// Alex Buzmion II 2025

#pragma once

#include "HTTPRequestTypes.generated.h"

namespace HTTPStatusMessages
{
	extern DEDICATEDSERVERS_API const FString SomethingWentWrong; 	
}

USTRUCT()
struct FDS_MetaData
{
	GENERATED_BODY()
// meta data from the received response. property names need to reflect the exact meta data names received from the API call 
	UPROPERTY()
	int32 httpStatusCode{};

	UPROPERTY()
	FString requestId{};

	UPROPERTY()
	int32 attempts{};

	UPROPERTY()
	double totalRetryDelay{};

	void Dump() const;
};

USTRUCT()
struct FDS_ListFleetsResponse
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FString> FleetIds{};

	UPROPERTY()
	FString NextToken{};

	void Dump() const;
};

USTRUCT()
struct FDS_GameSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString CreationTime{};
	
	UPROPERTY()
	FString CreatorId{};

	UPROPERTY()
	int32 CurrentPlayerSessionCount{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	TMap<FString, FString> GameProperties{};

	UPROPERTY()
	FString GameSessionData{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString Location{};

	UPROPERTY()
	FString MatchmakerData{};

	UPROPERTY()
	int32 MaximumPlayerSessionCount{};

	UPROPERTY()
	FString Name{};

	UPROPERTY()
	FString PlayerSessionCreationPolicy{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString StatusReason{};

	UPROPERTY()
	FString TerminationTime{};

	void Dump() const;
};

USTRUCT()
struct FDS_PlayerSession
{
	GENERATED_BODY()

	UPROPERTY()
	FString PlayerSessionId{};

	UPROPERTY()
	FString PlayerId{};

	UPROPERTY()
	FString GameSessionId{};

	UPROPERTY()
	FString FleetId{};

	UPROPERTY()
	FString FleetArn{};

	UPROPERTY()
	FString CreationTime{};

	UPROPERTY()
	FString TerminationTime{};

	UPROPERTY()
	FString Status{};

	UPROPERTY()
	FString IpAddress{};

	UPROPERTY()
	FString DnsName{};

	UPROPERTY()
	int32 Port{};

	UPROPERTY()
	FString PlayerData{};

	void Dump() const;
};

USTRUCT()
struct FDS_NewDeviceMetadata
{
	GENERATED_BODY()

	UPROPERTY()
	FString DeviceGroupKey{};

	UPROPERTY()
	FString DeviceKey{};

	void Dump() const;
};

USTRUCT()
struct FDS_AuthenticationResult
{
	GENERATED_BODY()

	UPROPERTY()
	FString AccessToken{};

	UPROPERTY()
	int32 ExpiresIn{};

	UPROPERTY()
	FString IdToken{};

	UPROPERTY()
	FDS_NewDeviceMetadata NewDeviceMetadata{};

	UPROPERTY()
	FString RefreshToken{};

	UPROPERTY()
	FString TokenType{};

	void Dump() const;
};

USTRUCT()
struct FDS_InitiateAuthResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FDS_AuthenticationResult AuthenticationResult{};

	UPROPERTY()
	TArray<FString> AvailableChallenges{};

	UPROPERTY()
	FString ChallengeName{};

	UPROPERTY()
	TMap<FString, FString> ChallengeParameters{};

	UPROPERTY()
	FString Session{};
	
	UPROPERTY()
	FString Email{};
	
	void Dump() const;
};


USTRUCT()
struct FDS_CodeDeliveryDetails
{
	GENERATED_BODY()

	UPROPERTY()
	FString AttributeName{};

	UPROPERTY()
	FString DeliveryMedium{};

	UPROPERTY()
	FString Destination{};

	void Dump() const;
};

USTRUCT()
struct FDS_SignUpResponse
{
	GENERATED_BODY()

	UPROPERTY()
	FDS_CodeDeliveryDetails CodeDeliveryDetails{};

	UPROPERTY()
	FString Session{};

	UPROPERTY()
	bool UserConfirmed{};

	UPROPERTY()
	FString UserSub{};

	void Dump() const;
};

USTRUCT()
struct FDS_PlayerData
{
	GENERATED_BODY()

	UPROPERTY()
	int32 level{};

	UPROPERTY()
	int32 health{};

	UPROPERTY()
	int32 isAlive;

	UPROPERTY()
	FString lastMapSaved; 
};

USTRUCT()
struct FDS_RecordPlayerDataInput
{
	GENERATED_BODY()

	UPROPERTY()
	FDS_PlayerData playerData{};

	UPROPERTY()
	FString username{};
};

USTRUCT()
struct FDS_RetrievePlayerDataResponse
{
	GENERATED_BODY()

	UPROPERTY(meta = (JsonProperty = "level"))
	int32 Level {};
	
	UPROPERTY()
	int32 health{};

	UPROPERTY()
	int32 isAlive {};
	
	UPROPERTY()
	FString lastMapSaved {};

	UPROPERTY()
	FString databaseid{};
	
	UPROPERTY()
	FString username{};
	
	UPROPERTY()
	FString email{};
	
	void Dump() const;
};

USTRUCT()
struct FDS_CharacterDetails
{
	GENERATED_BODY()

	UPROPERTY()
	FString characterName {};

	UPROPERTY()
	FString characterClass {};

	UPROPERTY()
	int32 isAlive {};

	UPROPERTY()
	FString lastMapLocation {};

	UPROPERTY()
	int32 level {};
};

USTRUCT()
struct FDS_GetCharactersResponse
{
	GENERATED_BODY()

	// Map to hold multiple characters (e.g., char1, char2, etc.)
	UPROPERTY()
	TMap<FString, FDS_CharacterDetails> characters {};
};