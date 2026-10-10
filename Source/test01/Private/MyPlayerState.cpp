// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"
// 构造函数：在 PlayerState 上创建玩家的 GAS 组件与属性集
AMyPlayerState::AMyPlayerState()
{
	// 创建 GAS 组件
	AbilitySystem = CreateDefaultSubobject<UMyAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetIsReplicated(true);   // 允许复制到客户端
	// Mixed 模式：属性(Attribute)全量复制 + GE 最小复制(适合单机/轻量网络)
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// 创建属性集(生命/法力等)
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>("MyAttributeSet");
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}


