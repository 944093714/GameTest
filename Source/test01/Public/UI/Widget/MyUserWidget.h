// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 用户控件基类。
 * 通用的控件控制器（WidgetController）挂载入口，供所有需要绑定数据的 UI 控件继承。
 */
UCLASS()
class TEST01_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/**
	 * 设置并绑定该控件的控制器（WidgetController）。
	 * @param InWidgetController 传入的控制器对象，会被保存并触发蓝图中实现的 WidgetControllerSet()
	 */
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject * InWidgetController);

	/** 当前绑定的控件控制器，保存外部传入的控制器对象以便后续访问 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	/**
	 * 控制器设置完成的事件（蓝图实现）。
	 * 当 SetWidgetController 被调用后触发，用于在 BP 中绑定属性到 UI 上。
	 */
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};

