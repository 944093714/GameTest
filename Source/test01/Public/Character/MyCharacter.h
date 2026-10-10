// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "MyCharacter.generated.h"

/**
 * 玩家角色类
 * 继承自 AMyBaseCharacter（自带武器+GAS 组件槽位）
 * 负责：设置移动朝向方式，以及在服务器/客户端两侧把 GAS
 *       绑定到 PlayerState 上创建的 AbilitySystem 组件与属性集上。
 */
UCLASS()
class TEST01_API AMyCharacter : public AMyBaseCharacter
{
	GENERATED_BODY()
public:
	AMyCharacter();
	virtual void PossessedBy(AController* NewController) override;   // 服务器端：被玩家控制器接管时调用
	virtual void OnRep_PlayerState() override;                       // 客户端：PlayerState 复制到达时调用

private:
	void SetPlayerInfo();   // 把 GAS 组件信息绑定到本角色（服务器/客户端共用）
};
