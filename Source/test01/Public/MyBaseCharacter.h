// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interface/CombatInterface.h"
#include "MyBaseCharacter.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;
/**
 * 角色基类
 * 同时继承 ACharacter(运动/骨骼网格) 与 IAbilitySystemInterface(暴露 GAS 组件)。
 * 玩家(AMyCharacter)与敌人(AMyEnemy)都继承它。
 */
UCLASS()
class TEST01_API AMyBaseCharacter : public ACharacter, public IAbilitySystemInterface,public ICombatInterface
{
	GENERATED_BODY()

public:
	AMyBaseCharacter();                          // 构造函数：创建绑定到右手骨骼插槽的武器子对象
	virtual void BeginPlay() override;           // 出生时调用（子类可覆写用于初始化 GAS 等）
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;   // 实现 GAS 接口
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }                // 内联取属性集
	
	// 武器尖端插槽名：用于定位伤害/生成点(如子弹)
	UPROPERTY(EditAnywhere, Category = "Combat")
	FName WeaponTipSocketName;
	// 获取武器尖端插槽的世界坐标(用于瞄准/生成等)
	virtual FVector GetCombatSocketLocation() override;
	
protected:
	// 右手武器(实际挂在骨骼插槽 WeaponHandSocket 上)
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	// GAS 组件(玩家由 PlayerState 提供并绑定; 敌人在自身创建)
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;
	// 属性集(同上，来源因类而异)
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	// 初始属性 GE：在 InitializePrimaryAttributes 中被施加到自身，写入生命/法力等默认值
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	// 初始化角色基础属性：把 DefaultPrimaryAttributes 对应的 GE 施加到自身
	void InitializePrimaryAttributes() const;

	// 启动技能：把 StartupAbilities 里配置的技能注册进 GAS 组件（仅服务器端执行）
	void AddCharacterAbilities() const;
private: 
	
	// 启动技能列表：出生时批量授予的 GameplayAbility，可在蓝图/编辑器中配置
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
};
