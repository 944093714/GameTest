// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayMyWidgetController.h"

#include "AbilitySystem/MyAttributeSet.h"

void UOverlayMyWidgetController::BroadcastInitialValues()
{
	const UMyAttributeSet* MyAttributeSet = Cast<UMyAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(MyAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(MyAttributeSet->GetMaxHealth());
}

void UOverlayMyWidgetController::BindCallbacksToDependancies()
{
	const UMyAttributeSet* MyAttributeSet = Cast<UMyAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MyAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayMyWidgetController::HealthChanged);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		MyAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayMyWidgetController::MaxHealthChanged);
	
}

void UOverlayMyWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayMyWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
