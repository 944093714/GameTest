// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * 集中管理的 Native GameplayTags（UE 官方宏，GAS/Lyra 通用做法）。
 * 每个标签是一个 FGameplayTag 全局常量，在 cpp 里用 UE_DEFINE_GAMEPLAY_TAG
 * 声明并自动注册（无需手动调用初始化）。使用处直接写标签名即可。
 */
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LMB)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_RMB)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_1)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_2)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_3)
UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_4)