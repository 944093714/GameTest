// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyInterface.h"
#include "MyPlayerController.generated.h"
class UMyAbilitySystemComponent;
class UMyEnhancedInputComponent;
struct FGameplayTag;
class UMyInputConfig;
class UInputMappingContext; // 前置声明：增强输入上下文
class UInputAction; // 前置声明：输入动作
struct FInputActionValue; // 前置声明：输入动作读取的数值
class USplineComponent;
/**
 * 玩家控制器类
 * 负责：增强输入(移动)、鼠标追踪(CursorTrace) 以高亮敌人、初始化输入映射与鼠标/输入模式。
 */
UCLASS()

class TEST01_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController(); // 构造函数
	virtual void PlayerTick(float DeltaTime) override; // 每帧调用 → 驱动鼠标追踪
protected:
	virtual void BeginPlay() override; // 注册输入映射、显示鼠标
	virtual void SetupInputComponent() override; // 把移动动作绑定到 Move()

private:
	// 增强输入上下文（按键→动作的映射表）
	UPROPERTY(EditAnywhere, Category = "InputComponent")
	TObjectPtr<UInputMappingContext> MyInputMappingContext;

	// 移动输入动作
	UPROPERTY(EditAnywhere, Category = "InputComponent")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue); // 相对视角方向的移动
	void CursorTrace(); // 鼠标射线追踪 → 切换高亮
	FHitResult CursorHit;
	IMyInterface* LastActor; // 上一帧命中的接口对象
	IMyInterface* ThisActor; // 当前帧命中的接口对象

	//三个回调函数通过BindAbilityActions 进行绑定
	// 能力输入回调：当绑定了对应能力标签的按键被「按下」时触发
	void AbilityInputTagPressed(FGameplayTag InputTag);
	// 能力输入回调：当绑定了对应能力标签的按键被「松开」时触发
	void AbilityInputTagReleased(FGameplayTag InputTag);
	// 能力输入回调：当绑定了对应能力标签的按键被「持续按住」时触发
	void AbilityInputTagHeld(FGameplayTag InputTag);

	// 自定义输入配置资产：用于把按键动作绑定到对应的能力标签(InputTag)
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UMyInputConfig> MyInputConfig;

	UPROPERTY()
	TObjectPtr<UMyAbilitySystemComponent> MyAbilitySystemComponent;
	// 获取 ASC（懒加载，首次调用时从 Pawn 上取得）
	UMyAbilitySystemComponent* GetASC();

	// —— 鼠标点击移动/自动奔跑(CursorTrace)状态相关 ——
	FVector CachedDestination = FVector::ZeroVector; // 当前地面点击的目标点（被持续跟随）
	float FollowTime = 0.f; // 按住左键移动的累计时长，用于区分短按/长按
	float ShortPressThreshold = 0.5f; // 短按阈值：低于此值视为单击（用于自动寻路/释放技能）
	bool bAutoRunning = false; // 是否处于沿样条自动奔跑状态
	bool bTargeting = false; // 当前是否悬停/瞄准在可交互对象上

	// 自动奔跑到达终点时允许的误差半径
	UPROPERTY(EditAnywhere)
	float AutoRunAcceptanceRadius = 50.f;

	// 自动奔跑所要跟随的路径样条组件
	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> Spline;

	// 自动奔跑逻辑：沿样条朝目标移动，到达误差半径后停止
	void AutoRun();
	void IsAutoRun();
	void PressRun();
};
