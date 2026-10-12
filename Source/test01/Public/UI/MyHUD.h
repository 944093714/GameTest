// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayMyWidgetController;
class UMyUserWidget;
/**
 * HUD 类：负责创建并装配「覆盖层 UI(Overlay)」。
 * 在玩家 GAS 初始化完成后，由 AMyCharacter 调用 InitOverlay()，
 * 创建覆盖层控件及其控件控制器，并把 GAS/PlayerState 等信息注入其中。
 */
UCLASS()
class TEST01_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	/** 当前创建的覆盖层控件（HUD 持有的 HUD Widget 实例） */
	UPROPERTY()
	TObjectPtr<UMyUserWidget> OverlayWidget;

	/** 获取覆盖层控件控制器；若尚未创建则先创建并完成参数注入与依赖绑定（懒加载） */
	UOverlayMyWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& Params);

	/** 初始化覆盖层：创建控件 + 控制器，把 PC/PS/GAS/属性集注入，并广播初始值到 UI */
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

protected:
	virtual void BeginPlay() override;

private:
	/** 覆盖层控件类（在编辑器中指定，Base 设为 MyUserWidget） */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMyUserWidget> OverlayWidgetClass;

	/** 覆盖层控件控制器实例（懒创建，GetOverlayWidgetController 时生成） */
	UPROPERTY()
	TObjectPtr<UOverlayMyWidgetController> OverlayWidgetController;

	/** 覆盖层控件控制器类（在编辑器中指定） */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayMyWidgetController> OverlayWidgetControllerClass;
};
