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

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	// 默认构造函数（供反射/蓝图使用）
	FWidgetControllerParams()
	{
	}

	// 带参构造函数：一次性接收控制器所需的四项数据
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS)
	{
	}

	/** 所属玩家的 PlayerController，用于控制与输入相关操作 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	/** 所属玩家的 PlayerState，用于获取属性、分数等状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	/** 玩家的能力系统组件（GAS），用于技能与效果相关数据 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	/** 玩家的属性集（GAS），用于读取生命、法力等基础属性 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
};

UCLASS()
class TEST01_API UMyWidgetController : public UObject
{
	GENERATED_BODY()

public:
	/** 设置该控制器的四项成员（PC/PS/GAS/属性集），供蓝图调用 */
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);
	/** 向绑定的 UI 广播各属性的初始值（一般由 HUD 首次创建控件时调用） */
	virtual void BroadcastInitialValues();
	/** 订阅 GAS 属性变化委托，使 UI 在属性变化时持续更新（子类重写） */
	virtual void BindCallbacksToDependancies();
protected:
	/** 所属玩家的 PlayerController，用于控制与输入相关操作 */
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerController> PlayerController;

	/** 所属玩家的 PlayerState，用于获取玩家属性、分数等状态 */
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<APlayerState> PlayerState;

	/** 玩家的能力系统组件（GAS），用于技能与效果相关的数据 */
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	/** 玩家的属性集（GAS），用于读取生命、法力等基础属性 */
	UPROPERTY(BlueprintReadOnly, Category="WidgetController")
	TObjectPtr<UAttributeSet> AttributeSet;
};
