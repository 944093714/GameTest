// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/WidgetController/OverlayMyWidgetController.h"
#include "UI/Widget/MyUserWidget.h"

// 获取覆盖层控制器（懒加载）：若尚未创建，则按指定的类实例化，注入参数并订阅属性变化
UOverlayMyWidgetController* AMyHUD::GetOverlayWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayWidgetController == nullptr)
	{
		// 创建控制器实例，把 PC/PS/GAS/属性集注入
		OverlayWidgetController = NewObject<UOverlayMyWidgetController>(this, OverlayWidgetControllerClass);

		OverlayWidgetController->SetWidgetControllerParams(Params);
		// 控制器内部绑定属性变化委托（此后属性一变即可转发入 UI）
		OverlayWidgetController->BindCallbacksToDependancies();

		return OverlayWidgetController;
	}

	// 已存在则直接复用
	return OverlayWidgetController;
}

// 初始化覆盖层 UI：创建控件 + 控制器，建立「控件 <-> 控制器 <-> GAS」的完整链路
void AMyHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 校验编辑器里必须已指定控件类与控制器类
	checkf(OverlayWidgetClass, TEXT("Overlay widget"))
	checkf(OverlayWidgetControllerClass, TEXT("Overlay widget Controller"));

	// 1) 创建覆盖层控件
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UMyUserWidget>(Widget);

	// 2) 打包参数并获取（懒创建）控制器
	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayMyWidgetController* Controller = GetOverlayWidgetController(WidgetControllerParams);

	// 3) 把控制器挂到控件上（MyUserWidget 事件在蓝图里把数据绑到具体 UI）
	OverlayWidget->SetWidgetController(Controller);
	// 4) 广播一次初始值，让 HUD 立即显示当前生命等数值
	Controller->BroadcastInitialValues();

	// 5) 添加到视口显示
	Widget->AddToViewport();
}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();
}
