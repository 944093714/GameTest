// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MyInterface.generated.h"

/**
 * UMyInterface：UE 反射包裹类(MinimalAPI=最小导出)，本身无需修改。
 * 真正的接口本体是下面的 IMyInterface。
 */
UINTERFACE(MinimalAPI)
class UMyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * IMyInterface：高亮接口
 * 任何需要被玩家鼠标「悬停高亮」的 Actor(如敌人)都应实现本接口。
 */
class TEST01_API IMyInterface
{
	GENERATED_BODY()

	// 需要被实现类继承并实现的接口函数
public:
	virtual void HighlightActor() = 0;       // 进入悬停范围：触发高亮
	virtual void UnHighlightActor() = 0;     // 离开悬停范围:取消高亮
};
