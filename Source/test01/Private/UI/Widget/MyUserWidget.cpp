// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/MyUserWidget.h"

void UMyUserWidget::SetWidgetController(UObject* InWidgetController)
{
	// 记录传入的控制器对象
	WidgetController = InWidgetController;
	// 触发蓝图实现的事件，用于将控制器的数据绑定到控件上
	WidgetControllerSet();
}
