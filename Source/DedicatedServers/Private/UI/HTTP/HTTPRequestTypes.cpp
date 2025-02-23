// Alex Buzmion II 2025

#include "UI/HTTP/HTTPRequestTypes.h"
#include "DedicatedServers/DedicatedServers.h"

namespace HTTPStatusMessages
{
	const FString SomethingWentWrong{TEXT("Something went wrong!")};	
}

void FDS_MetaData::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("MetaData: "));		
	UE_LOG(LogDedicatedServers, Log, TEXT("httpStatusCode: %d "), httpStatusCode);		
	UE_LOG(LogDedicatedServers, Log, TEXT("requestId: %s"), *requestId);		
	UE_LOG(LogDedicatedServers, Log, TEXT("attempts: %d"), attempts);		
	UE_LOG(LogDedicatedServers, Log, TEXT("totalRetryDelay: %f"), totalRetryDelay);		
}

void FDS_ListFleetsResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("List Fleet Response: "));		
	for (const FString& FleetId : FleetIds)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);		
	}
	if (!NextToken.IsEmpty())
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("NextToken: %s"), *NextToken);	
	}
}

void FDS_GameSession::Dump() const {
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSession: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSession: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("CreatorId: %s"), *CreatorId);
	UE_LOG(LogDedicatedServers, Log, TEXT("CurrentPlayerSessionCount: %d"), CurrentPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
    
	// TMap GameProperties
	for (const auto& Pair : GameProperties) {
		UE_LOG(LogDedicatedServers, Log, TEXT("GameProperties - Key: %s, Value: %s"), *Pair.Key, *Pair.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionData: %s"), *GameSessionData);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("Location: %s"), *Location);
	UE_LOG(LogDedicatedServers, Log, TEXT("MatchmakerData: %s"), *MatchmakerData);
	UE_LOG(LogDedicatedServers, Log, TEXT("MaximumPlayerSessionCount: %d"), MaximumPlayerSessionCount);
	UE_LOG(LogDedicatedServers, Log, TEXT("Name: %s"), *Name);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionCreationPolicy: %s"), *PlayerSessionCreationPolicy);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("StatusReason: %s"), *StatusReason);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime: %s"), *TerminationTime);
}

void FDS_PlayerSession::Dump() const {
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSession Data: "));
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerSessionId: %s"), *PlayerSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerId: %s"), *PlayerId);
	UE_LOG(LogDedicatedServers, Log, TEXT("GameSessionId: %s"), *GameSessionId);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetId: %s"), *FleetId);
	UE_LOG(LogDedicatedServers, Log, TEXT("FleetArn: %s"), *FleetArn);
	UE_LOG(LogDedicatedServers, Log, TEXT("CreationTime: %s"), *CreationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("TerminationTime: %s"), *TerminationTime);
	UE_LOG(LogDedicatedServers, Log, TEXT("Status: %s"), *Status);
	UE_LOG(LogDedicatedServers, Log, TEXT("IpAddress: %s"), *IpAddress);
	UE_LOG(LogDedicatedServers, Log, TEXT("DnsName: %s"), *DnsName);
	UE_LOG(LogDedicatedServers, Log, TEXT("Port: %d"), Port);
	UE_LOG(LogDedicatedServers, Log, TEXT("PlayerData: %s"), *PlayerData);
}

void FDS_NewDeviceMetadata::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("New Device Metadata:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("DeviceGroupKey: %s"), *DeviceGroupKey);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeviceKey: %s"), *DeviceKey);	
}

void FDS_AuthenticationResult::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Authentication Result:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("AccessToken: %s"), *AccessToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("ExpiresIn: %d"), ExpiresIn);
	UE_LOG(LogDedicatedServers, Log, TEXT("IdToken: %s"), *IdToken);
	NewDeviceMetadata.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("RefreshToken: %s"), *RefreshToken);
	UE_LOG(LogDedicatedServers, Log, TEXT("TokenType: %s"), *TokenType);
}

void FDS_InitiateAuthResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Initiate Auth Response:"));
	AuthenticationResult.Dump();

	for (const FString& Challenge : AvailableChallenges)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("AvailableChallenge: %s"), *Challenge);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("ChallengeName: %s"), *ChallengeName);

	for (const TPair<FString, FString>& Param : ChallengeParameters)
	{
		UE_LOG(LogDedicatedServers, Log, TEXT("ChallengeParameter: %s -> %s"), *Param.Key, *Param.Value);
	}

	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
}

void FDS_CodeDeliveryDetails::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Code Delivery Details:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("AttributeName: %s"), *AttributeName);
	UE_LOG(LogDedicatedServers, Log, TEXT("DeliveryMedium: %s"), *DeliveryMedium);
	UE_LOG(LogDedicatedServers, Log, TEXT("Destination: %s"), *Destination);	
}

void FDS_SignUpResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("Sign Up Response:"));
	CodeDeliveryDetails.Dump();
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %s"), *Session);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), UserConfirmed ? TEXT("true") : TEXT("false"));
	UE_LOG(LogDedicatedServers, Log, TEXT("UserSub: %s"), *UserSub);	
}

void FDS_RetrievePlayerDataResponse::Dump() const
{
	UE_LOG(LogDedicatedServers, Log, TEXT("RetrievePlayer Data Response:"));
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %d"), Level);
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %d"), health);
	UE_LOG(LogDedicatedServers, Log, TEXT("Session: %d"), isAlive);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), *lastMapSaved);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), *username);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), *email);
	UE_LOG(LogDedicatedServers, Log, TEXT("UserConfirmed: %s"), *databaseid);
}
