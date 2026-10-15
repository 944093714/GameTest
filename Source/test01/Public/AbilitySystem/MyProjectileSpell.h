// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MyGameplayAbility.h"
#include "MyProjectileSpell.generated.h"

class AMyProjectile;
/**
 * 投射物技能类：激活时在施法者的战斗插槽(Socket)位置生成一枚投射物子弹。
 * 继承自 UMyGameplayAbility，作为"发射子弹类技能"的通用基类。
 */
UCLASS()
class TEST01_API UMyProjectileSpell : public UMyGameplayAbility
{
	GENERATED_BODY()
protected:
	// 重写激活回调：技能被触发时生成并发射子弹
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	// 要生成的子弹类的蓝图/C++ 类，在编辑器中配置
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMyProjectile> ProjectileClass;


};
