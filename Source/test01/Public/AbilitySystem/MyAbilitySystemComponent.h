// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

/**
 * 本项目专用的 GAS 组件子类
 * 目前为空，直接继承引擎 UAbilitySystemComponent，作为后续扩展点(原生技能、预激活等)。
 */
UCLASS()
class TEST01_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	/**
	 * 为角色批量授予初始游戏玩法能力。
	 * @param StartAbilities 待授予的能力类数组(以 TSubclassOf 方式传入能力蓝图/C++ 类)
	 * @note 每个能力会被生成对应的 AbilitySpec 并立即授予(并激活执行一次)到该组件，
	 *        通常在角色初始化能力时调用一次。
	 */
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartAbilities);
	
};
