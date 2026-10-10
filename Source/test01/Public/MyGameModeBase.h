// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 游戏模式类
 * 暂为空，默认 AGameModeBase 逻辑；后续可在此指定默认 Pawn / PlayerController /
 * GameState，或在蓝图中设置玩家控制器的默认类型为 AMyPlayerController。
 */
UCLASS()
class TEST01_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

};
