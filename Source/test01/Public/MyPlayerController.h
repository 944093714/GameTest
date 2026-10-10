// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyInterface.h"
#include "MyPlayerController.generated.h"
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
/**
 * 玩家控制器类
 * 负责：增强输入(移动)、鼠标追踪(CursorTrace) 以高亮敌人、初始化输入映射与鼠标/输入模式。
 */
UCLASS()

class TEST01_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AMyPlayerController();
	virtual void PlayerTick(float DeltaTime) override;   // 每帧调用 → 驱动鼠标追踪
protected:
	virtual void BeginPlay() override;                    // 注册输入映射、显示鼠标
	virtual void SetupInputComponent() override;          // 把移动动作绑定到 Move()

private:
	// 增强输入上下文（按键→动作的映射表）
	UPROPERTY(EditAnywhere, Category = "InputComponent")
	TObjectPtr<UInputMappingContext> MyInputMappingContext;

	// 移动输入动作
	UPROPERTY(EditAnywhere, Category = "InputComponent")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);   // 相对视角方向的移动
	void CursorTrace();                                     // 鼠标射线追踪 → 切换高亮

	IMyInterface* LastActor;   // 上一帧命中的接口对象
	IMyInterface* ThisActor;   // 当前帧命中的接口对象
};
