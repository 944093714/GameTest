
#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "MyInterface.h"
#include "MyEnemy.generated.h"

/**
 * 敌人角色类
 * 继承自 AMyBaseCharacter（武器+GAS 槽位），并实现 IMyInterface 高亮接口，
 * 使玩家控制器 CursorTrace 命中它时可触发高亮/取消高亮。
 * 与玩家不同：敌人自己持有 GAS 组件与属性集（而非通过 PlayerState）。
 */
UCLASS()
class TEST01_API AMyEnemy : public AMyBaseCharacter, public IMyInterface
{
	GENERATED_BODY()
public:
	//敌人接口
	virtual void HighlightActor() override;      // 鼠标悬停 → 高亮
	virtual void UnHighlightActor() override;    // 鼠标移开 → 取消高亮
	//敌人接口结束

	AMyEnemy();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsHighlighted;   // 是否处于高亮状态（可在蓝图中读取/编辑）

};
