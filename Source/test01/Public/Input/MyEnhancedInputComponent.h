// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MyInputConfig.h"
#include "MyEnhancedInputComponent.generated.h"


/**
 * 
 */
UCLASS()
class TEST01_API UMyEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<class UserClass,typename PressedFuncType,typename ReleasedFuncType,typename HeldFuncType>
	void BindAbilityActions(const UMyInputConfig* InputConfig,UserClass* Object,PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc,HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UMyEnhancedInputComponent::BindAbilityActions(const UMyInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	for (const FMyInputAction& Action:InputConfig->MyInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,PressedFunc,Action.InputTag);
			}
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,ReleasedFunc,Action.InputTag);
			}
			if (HeldFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,HeldFunc,Action.InputTag);
			}
		}
	}
}
