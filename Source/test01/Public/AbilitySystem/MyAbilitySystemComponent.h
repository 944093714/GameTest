// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MyAbilitySystemComponent.generated.h"

/**
 * 本项目专用的 GAS 组件子类
 * 目前为空，直接继承引擎 UAbilitySystemComponent，作为后续扩展点(原生技能、预激活等)。
 */
UCLASS()
class TEST01_API UMyAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

};
