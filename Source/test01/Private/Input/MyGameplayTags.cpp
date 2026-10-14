// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MyGameplayTags.h"

// 定义并自动注册为 native 标签（TagForm 用点做层级分隔，Comment 为注释）
UE_DEFINE_GAMEPLAY_TAG(InputTag_LMB, "InputTag.LMB", "鼠标左键")
UE_DEFINE_GAMEPLAY_TAG(InputTag_RMB, "InputTag.RMB", "鼠标右键")
UE_DEFINE_GAMEPLAY_TAG(InputTag_1, "InputTag.1", "数字键1")
UE_DEFINE_GAMEPLAY_TAG(InputTag_2, "InputTag.2", "数字键2")
UE_DEFINE_GAMEPLAY_TAG(InputTag_3, "InputTag.3", "数字键3")
UE_DEFINE_GAMEPLAY_TAG(InputTag_4, "InputTag.4", "数字键4")