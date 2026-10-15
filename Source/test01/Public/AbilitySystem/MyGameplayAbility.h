// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyGameplayAbility.generated.h"

/**
 * 自定义「主动/被动」游戏玩法能力基类。
 *
 * 继承自引擎 UGameplayAbility，作为本项目所有 GameplayAbility 的统一基类，
 * 用于后续集中扩展能力相关的通用逻辑，例如：
 *   - 统一的冷exchange/消耗处理
 *   - 技能对象的归属(施法者)取值
 *   - 伤害计算与命中表现的通用入口
 *
 * 当前版本尚未重写任何逻辑，直接使用引擎能力基类的默认行为；
 * 后续新能力可继承此类以获得项目级定制能力。
 */
UCLASS()
class TEST01_API UMyGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	// 该技能的起始输入标签：在蓝图默认值中配置，用于标识按下哪个输入键可触发此技能
	UPROPERTY(EditDefaultsOnly,Category="Input")
	FGameplayTag StartInputTag;
};
