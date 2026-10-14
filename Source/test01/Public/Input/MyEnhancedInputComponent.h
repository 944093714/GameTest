// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "MyInputConfig.h"
#include "MyEnhancedInputComponent.generated.h"


/**
 * UMyEnhancedInputComponent
 * 继承 UEnhancedInputComponent，扩展了一个按配置批量绑定输入动作的模板方法。
 * 遍历 UMyInputConfig 中配置的 InputAction↔GameplayTag 映射，
 * 把每个输入动作绑定到传入的回调，并额外把对应的 GameplayTag 作为参数传进去，
 * 便于在 GAS 中通过 AbilityTag 分发技能。
 */
UCLASS()
class TEST01_API UMyEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/**
	 * 按输入配置批量绑定“输入动作 → 游戏标签”的回调（模板函数，声明和实现都在头文件以支持模板实例化）。
	 * @param InputConfig	输入映射配置数据资产（含 MyInputActions 数组）
	 * @param Object		回调绑定的目标对象（通常是拥有该组件的 Pawn/PlayerController）
	 * @param PressedFunc	按下（触发 Started 事件）时的回调
	 * @param ReleasedFunc	松开（触发 Completed 事件）时的回调
	 * @param HeldFunc		持续按住（触发 Triggered 事件）时的回调
	 */
	template<class UserClass,typename PressedFuncType,typename ReleasedFuncType,typename HeldFuncType>
	void BindAbilityActions(const UMyInputConfig* InputConfig,UserClass* Object,PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc,HeldFuncType HeldFunc);
};

/**
 * 批量绑定实现：遍历配置的每个输入动作，绑定按/松/持续三种状态回调，
 * 并把对应的 GameplayTag 作为额外参数传给回调（回调签名通常带有 FGameplayTag 形参）。
 */
template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UMyEnhancedInputComponent::BindAbilityActions(const UMyInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc,ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	// 遍历数据资产中配置的所有（InputAction, InputTag）映射条目
	for (const FMyInputAction& Action:InputConfig->MyInputActions)
	{
		// 输入动作有效且标签合法时才进行绑定
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			// 若提供了按下回调，则绑定动作的 Started 触发事件，并附带该输入的 GameplayTag
			if (PressedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,PressedFunc,Action.InputTag);
			}
			// 若提供了松开回调，则绑定动作的 Started 触发事件（注：此处事件类型与实际回调语义可能需按需核对）
			if (ReleasedFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,ReleasedFunc,Action.InputTag);
			}
			// 若提供了持续按住回调，则绑定动作的 Started 触发事件
			if (HeldFunc)
			{
				BindAction(Action.InputAction,ETriggerEvent::Started,Object,HeldFunc,Action.InputTag);
			}
		}
	}
}
