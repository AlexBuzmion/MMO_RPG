// Alex Buzmion II 2025

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "CherubInputConfig.h"
#include "CherubInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MMO_GAME_API UCherubInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	// template function capable of receiving functions or function pointers of any type
	// takes in a function and an input that will be passed along into those callbacks when input is pressed, released or held
	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UCherubInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc); 
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UCherubInputComponent::BindAbilityActions(const UCherubInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	check(InputConfig);
	for ( const FCherubInputAction& action : InputConfig->AbilityInputActions)
	{ 
		if (action.InputAction && action.InputTag.IsValid())
		{	// bind gameplay tags to the triggers 
			if (PressedFunc)
			{
				BindAction(action.InputAction, ETriggerEvent::Started, Object, PressedFunc, action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, action.InputTag);
			}
		}
	}
}
