// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MyInputConfig.h"

/**
 * 在映射数组中查找与指定 GameplayTag 匹配的 UInputAction。
 * @param InputTag		要查找的输入标签
 * @param bLogNotFound	找不到时是否打印错误日志
 * @return 匹配的 UInputAction 指针；未匹配返回 nullptr
 */
const UInputAction* UMyInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	// 遍历配置好的映射条目，逐条比对 InputTag
	for (const FMyInputAction& Action: MyInputActions)
	{
		// 仅当输入动作有效且标签匹配时才返回
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	// 未找到且调用方要求打印日志时，输出错误提示
	if (bLogNotFound)
	{
		UE_LOG(LogTemp,Error,TEXT("找不到与之对应的输入标签"))
	}
	return nullptr;
}
