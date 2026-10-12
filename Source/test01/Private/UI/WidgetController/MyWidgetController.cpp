// Fill out your copyright notice in the Description page of Project Settings.


// 控件控制器基类实现文件（暂无实现逻辑）
#include "UI/WidgetController/MyWidgetController.h"

void UMyWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}
