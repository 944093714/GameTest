// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MyInputConfig.h"

const UInputAction* UMyInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	
	for (const FMyInputAction& Action: MyInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("找不到与之对应的输入标签"))
	}
	return nullptr;
}
