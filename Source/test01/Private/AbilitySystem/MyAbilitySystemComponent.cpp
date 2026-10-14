// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyAbilitySystemComponent.h"

// 暂无额外实现：完全继承引擎 UAbilitySystemComponent 的默认功能。

// 批量授予初始能力：遍历传入的能力类列表，逐个生成 AbilitySpec 并授予到该组件
void UMyAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartAbilities)
{
	// 遍历每一个待授予的能力类
	for (TSubclassOf<UGameplayAbility> Ability : StartAbilities)
	{
		// 以 1 级创建一个能力规格(AbilitySpec)，区分一个能力类的多个实例(如不同等级/来源的同一技能)
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,1.f);
		// GiveAbility(AbilitySpec);           // (被注释)原先仅授予但不激活；创建后不会自动触发
		// 授予并立即激活一次：能力被加上后马上执行一次，适用于主动触发的 instant/auto 技能
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}
