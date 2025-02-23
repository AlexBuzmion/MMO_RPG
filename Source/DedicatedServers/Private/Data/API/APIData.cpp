// Alex Buzmion II 2025


#include "Data/API/APIData.h"
FString UAPIData::GetAPIEndpoint(const FGameplayTag& APIEndpoint)
{
	const FString resourceName = Resources.FindChecked(APIEndpoint);

	return InvokeURL + "/" + StageName + "/" + resourceName;
}
