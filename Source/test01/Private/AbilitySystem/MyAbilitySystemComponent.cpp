  // Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyAbilitySystemComponent.h"

#include "AbilitySystem/MyGameplayAbility.h"

  // 暂无额外实现：完全继承引擎 UAbilitySystemComponent 的默认功能。

// =====================================================================
// 批量授予初始能力
// 遍历传入的能力类列表，逐个生成 AbilitySpec(能力规格) 并授予到该组件。
// =====================================================================
void UMyAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartAbilities)
{
	// 遍历每一个待授予的能力类
	for (const TSubclassOf<UGameplayAbility> Ability : StartAbilities)
	{
		// 以 1 级创建一个能力规格(AbilitySpec)。
		// AbilitySpec 用于区分同一个能力类的多个实例(不同等级 / 不同来源授予的同一技能)。
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability,1.f);

		// 只要当前能力是我们自定义的 UMyGameplayAbility 派生类，就取出其配置的输入标签并标记到规格上
		if (const UMyGameplayAbility* MyAbility = Cast<UMyGameplayAbility>(AbilitySpec.Ability))
		{
			// 把能力上配置的 StartInputTag 作为动态标签挂到 AbilitySpec 上，
			// 供后续根据输入按键(标签)精准检索到对应的能力。
			AbilitySpec.DynamicAbilityTags.AddTag(MyAbility->StartInputTag);
			// 正式将能力授予到本 GAS 组件，使其进入可激活状态
			GiveAbility(AbilitySpec);
		}

		//GiveAbilityAndActivateOnce(AbilitySpec); // 备选：授予后立即激活一次
	}
}

// =====================================================================
// 输入标签-持续按住回调
// 当某能力标签对应的按键被持续按住时，遍历本组件所有已授予的能力，
// 找出持有该标签的能力并发送"按下输入"信号；若它还不在激活状态，则尝试激活它。
// 适用于需要持续施放 / 蓄力的技能。
// =====================================================================
void UMyAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	// 传入空标签直接忽略
	if (!InputTag.IsValid()) return;

	// 遍历本组件上所有已授予(可激活)的能力规格，而非遍历输入标签本身
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// 判断该能力是否挂载了与当前按键一致的输入标签
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 告知引擎该能力对应的输入正被按住
			AbilitySpecInputPressed(AbilitySpec);
			// 若技能还没激活，则尝试触发它开始执行(按住的每一帧都尝试)
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

// =====================================================================
// 输入标签-松开回调
// 当某能力标签对应的按键松开时，遍历所有能力，向持有该标签的能力发送"输入已结束"信号。
// 适用于技能释放瞬间进行结算的能力。
// =====================================================================
void UMyAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	// 传入空标签直接忽略
	if (!InputTag.IsValid()) return;

	// 遍历所有可激活能力，找出持有该输入标签的能力规格
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			// 通知引擎该输入已被松开，供技能据此做释放结算
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}
