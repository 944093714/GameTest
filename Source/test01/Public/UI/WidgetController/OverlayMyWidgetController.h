// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/MyWidgetController.h"
#include "OverlayMyWidgetController.generated.h"

struct FOnAttributeChangeData;
// ===== HUD 覆盖层（Overlay）专用的动态多播委托 声明委托 =====
// 每声明一个「属性 → UI」的转发事件：当某属性变化时，由控制器把新值广播给绑定的蓝图控件。
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, Health); // 生命值变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, MaxHealth); // 最大生命值变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, Mana); // 法力值变化委托

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, MaxMana); // 最大法力值变化委托

/**
 * 覆盖层（Overlay HUD）控件控制器。
 * 继承自 UMyWidgetController，负责把玩家的「生命/最大生命/法力/最大法力」等关键属性
 * 从 GAS 转发到 UI 控件上：
 *  1. BroadcastInitialValues()       —— 控件创建时广播一次初始值
 *  2. BindCallbacksToDependancies()  —— 订阅属性变化委托，属性一变就继续转发
 * 可在蓝图中创建派生类，并把对应的委托（OnHealthChanged 等）绑定到具体的进度条/文本。
 */
UCLASS(BlueprintType, Blueprintable)
class TEST01_API UOverlayMyWidgetController : public UMyWidgetController
{
	GENERATED_BODY()

public:
	/** 向 UI 广播各项属性的初始值（控件首次打开时调用） */
	virtual void BroadcastInitialValues() override;
	/** 订阅 GAS 属性变化委托，让 UI 在属性变化时持续更新 */
	virtual void BindCallbacksToDependancies() override;

	/** 生命值变化时触发，携带最新生命值（BlueprintAssignable 便于在蓝图里绑定 UI） */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnHealthChangedSignature OnHealthChanged;

	/** 最大生命值变化时触发，携带最新最大生命值 */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;

	/** 法力值变化时触发，携带最新法力值（BlueprintAssignable 便于在蓝图里绑定 UI） */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnManaChangedSignature OnManaChanged;

	/** 最大法力值变化时触发，携带最新最大法力值 */
	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnMaxManaChangedSignature OnMaxManaChanged;

protected:
	/** 生命值变化回调：收到 GAS 新值后转发到 OnHealthChanged 委托 */
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	/** 最大生命值变化回调：转发到 OnMaxHealthChanged 委托 */
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	/** 法力值变化回调：转发到 OnManaChanged 委托 */
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	/** 最大法力值变化回调：转发到 OnMaxManaChanged 委托 */
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
};
