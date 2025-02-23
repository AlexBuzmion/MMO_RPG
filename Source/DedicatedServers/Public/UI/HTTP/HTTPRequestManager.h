// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HTTPRequestManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAPIStatusMsg, const FString&, StatusMessage, bool, bShouldResetWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAPIRequestSucceeded);

class UAPIData;
class FJsonObject;
class UDSLocalPlayerSubSystem;

UCLASS(Blueprintable)
class DEDICATEDSERVERS_API UHTTPRequestManager : public UObject
{
	GENERATED_BODY()

public:
	UDSLocalPlayerSubSystem* GetDSLocalPlayerSubSystem() const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAPIData> APIData;
	
	bool ContainsErrors(TSharedPtr<FJsonObject> JsonObject);
	void DumpMetaData(TSharedPtr<FJsonObject> JsonObject);

	static FString SerializeJsonObject(const TMap<FString, FString>& Params);
};
