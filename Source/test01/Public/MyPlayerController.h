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
	UMyAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditAnywhere)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USplineComponent> Spline;
	
	void AutoRun();
};
