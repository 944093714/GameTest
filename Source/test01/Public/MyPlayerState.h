// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 玩家状态类（玩家 GAS 的主机）
 * 把 GAS 组件与属性集放在 PlayerState 上，使它们跨角色重生保持存活、并支持网络复制。
 * 角色(AMyCharacter)通过本类拿到 GAS 组件完成初始化。
 */
UCLASS()
class TEST01_API AMyPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;   // 实现 GAS 接口
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }                // 内联取属性集

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;   // 玩家 GAS 组件

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;              // 玩家属性集(生命/法力)
};
