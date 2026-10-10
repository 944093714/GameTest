// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 控件控制器基类。
 * 负责为 UI 控件提供数据（Player、AbilitySystem 等），
 * 解耦 UI 与游戏逻辑，通常与 UMyUserWidget 搭配使用。
 */
UCLASS()
class TEST01_API UMyWidgetController : public UObject
{
	GENERATED_BODY()

protected:

	/** 所属玩家的 PlayerController，用于控制与输入相关操作 */
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	/** 所属玩家的 PlayerState，用于获取玩家属性、分数等状态 */
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	/** 玩家的能力系统组件（GAS），用于技能与效果相关的数据 */
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** 玩家的属性集（GAS），用于读取生命、法力等基础属性 */
	UPROPERTY(BlueprintReadOnly,Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
