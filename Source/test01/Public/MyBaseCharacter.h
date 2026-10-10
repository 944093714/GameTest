// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MyBaseCharacter.generated.h"


class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 角色基类
 * 同时继承 ACharacter(运动/骨骼网格) 与 IAbilitySystemInterface(暴露 GAS 组件)。
 * 玩家(AMyCharacter)与敌人(AMyEnemy)都继承它。
 */
UCLASS()
class TEST01_API AMyBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyBaseCharacter();
	virtual void BeginPlay() override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;   // 实现 GAS 接口
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }                // 内联取属性集
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
};
