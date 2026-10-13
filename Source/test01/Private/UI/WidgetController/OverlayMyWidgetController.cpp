// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayMyWidgetController.h"

#include "AbilitySystem/MyAttributeSet.h"

// 广播初始值：把属性集的当前值一次性推送给 UI（用于 HUD 控件刚创建时填充显示）
void UOverlayMyWidgetController::BroadcastInitialValues()
{
	const UMyAttributeSet* MyAttributeSet = Cast<UMyAttributeSet>(AttributeSet);

	// 注意广播顺序：先「最大值」再「当前值」，保证血条/蓝条在算百分比时最大值已就位
	OnMaxHealthChanged.Broadcast(MyAttributeSet->GetMaxHealth());
	OnHealthChanged.Broadcast(MyAttributeSet->GetHealth());

	OnMaxManaChanged.Broadcast(MyAttributeSet->GetMaxMana());
	OnManaChanged.Broadcast(MyAttributeSet->GetMana());
}

// 订阅依赖：把 GAS 中 Health / MaxHealth 属性的“变化委托”挂到本控制器的回调上，
// 之后任何属性改动都会自动触发下方回调并继续广播到 UI。
void UOverlayMyWidgetController::BindCallbacksToDependancies()
{
	const UMyAttributeSet* MyAttributeSet = Cast<UMyAttributeSet>(AttributeSet);

	// 生命值变化 → HealthChanged
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MyAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayMyWidgetController::HealthChanged);

	// 最大生命值变化 → MaxHealthChanged
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MyAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayMyWidgetController::MaxHealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MyAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayMyWidgetController::ManaChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MyAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayMyWidgetController::MaxManaChanged);

}

// 生命值变化回调：把 GAS 回调里的新值（Data.NewValue）转发到 UI 委托
void UOverlayMyWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

// 最大生命值变化回调：转发到 OnMaxHealthChanged 委托
void UOverlayMyWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayMyWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayMyWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

