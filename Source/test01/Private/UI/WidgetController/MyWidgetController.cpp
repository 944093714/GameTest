// Fill out your copyright notice in the Description page of Project Settings.


// 控件控制器基类实现文件（暂无实现逻辑）
#include "UI/WidgetController/MyWidgetController.h"

void UMyWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	// 把参数包中的四项数据依次赋值给本控制器的成员
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void UMyWidgetController::BroadcastInitialValues()
{
}

void UMyWidgetController::BindCallbacksToDependancies()
{
}
