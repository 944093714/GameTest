// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MyGameplayTags.generated.h"

/**
 * 基于 Native GameplayTag 的标签集合，集中管理输入/技能标签。
 * 在游戏加载早期（如模块 StartupModule 或 GameInstance::Init）调用
 * InitializeNativeGameplayTags() 完成注册，之后通过 Get() 全局引用。
 */
struct FMyGameplayTags
{
public:
	static const FMyGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();

	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

private:
	static FMyGameplayTags GameplayTags;
};

UCLASS()
class TEST01_API UMyGameplayTags : public UDataAsset
{
	GENERATED_BODY()
};
