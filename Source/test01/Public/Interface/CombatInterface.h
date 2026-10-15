// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
// UCombatInterface：UCLASS 装箱类，作为该接口的 UObject 反射对应物，仅此用途，无需修改
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * ICombatInterface
 * 战斗接口：定义战斗相关的通用功能，供各战斗角色（如玩家、敌人）实现。
 */
class TEST01_API ICombatInterface
{
	GENERATED_BODY()

public:
	/** 获取武器/攻击部位的挂点（Socket）世界位置，用于生成特效、子弹等出弹点 */
	virtual FVector GetCombatSocketLocation();
};
