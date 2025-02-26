// Alex Buzmion II 2025

#include "UI/HTTP/HTTPRequestManager.h"

#include "JsonObjectConverter.h"
#include "DedicatedServers/DedicatedServers.h"
#include "Player/DSLocalPlayerSubSystem.h"
#include "UI/HTTP/HTTPRequestTypes.h"

UDSLocalPlayerSubSystem* UHTTPRequestManager::GetDSLocalPlayerSubSystem() const {
	APlayerController* localPC = GEngine->GetFirstLocalPlayerController(GetWorld());
	if (!IsValid(localPC)) {
		UE_LOG(LogTemp, Warning, TEXT("GetDSLocalPlayerSubSystem: localPC is null"));
		return nullptr;
	}
    
	if (ULocalPlayer* localPlayer = Cast<ULocalPlayer>(localPC->Player))
	{
		if (IsValid(localPlayer)) {
			UDSLocalPlayerSubSystem* localPS = localPlayer->GetSubsystem<UDSLocalPlayerSubSystem>();
			if (IsValid(localPS))
			{
				return localPS;
			}
		}
	}
	return nullptr;
}

bool UHTTPRequestManager::ContainsErrors(TSharedPtr<FJsonObject> JsonObject)
{
	if (JsonObject->HasField(TEXT("errorType")) || JsonObject->HasField(TEXT("errorMessage")))
	{
		FString errorType = JsonObject->HasField(TEXT("errorType")) ? JsonObject->GetStringField(TEXT("errorType")) : TEXT("Unknown Error");
		FString errorMessage = JsonObject->HasField(TEXT("errorMessage")) ? JsonObject->GetStringField(TEXT("errorMessage")) : TEXT("Unknown Error Message");

		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s"), *errorType);
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Message: %s"), *errorMessage);

		return true; 
	}

	if (JsonObject->HasField(TEXT("$fault")))
	{
		FString fault = JsonObject->GetStringField(TEXT("$fault"));
		FString faultName = JsonObject->HasField(TEXT("name")) ? JsonObject->GetStringField(TEXT("name")) : TEXT("Unknown name");
		UE_LOG(LogDedicatedServers, Error, TEXT("Error Type: %s, Fault: %s"), *faultName, *fault);
		return true; 
	}
	return false;
}

void UHTTPRequestManager::DumpMetaData(TSharedPtr<FJsonObject> JsonObject) {
	if (JsonObject->HasField(TEXT("$metadata")))
	{
		TSharedPtr<FJsonObject> metaDataJsonObject = JsonObject->GetObjectField(TEXT("$metadata"));
		FDS_MetaData dsMetaData; 
		FJsonObjectConverter::JsonObjectToUStruct(metaDataJsonObject.ToSharedRef(), &dsMetaData);

		dsMetaData.Dump();
	}
}

FString UHTTPRequestManager::SerializeJsonObject(const TMap<FString, FString>& Params)
{
	TSharedPtr<FJsonObject> contentJsonObject = MakeShareable(new FJsonObject());
	
	for (const auto& param : Params)
	{
		contentJsonObject->SetStringField(param.Key, param.Value);
	}
	
	FString content;
	TSharedRef<TJsonWriter<>> jsonWriter = TJsonWriterFactory<>::Create(&content);
	FJsonSerializer::Serialize(contentJsonObject.ToSharedRef(), jsonWriter);

	return content;
}
