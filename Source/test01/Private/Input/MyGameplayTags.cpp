// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/MyGameplayTags.h"
#include "GameplayTagsManager.h"

FMyGameplayTags FMyGameplayTags::GameplayTags;

void FMyGameplayTags::InitializeNativeGameplayTags()
{
	// Lambda 便于批量声明原生标签，避免重复注册
	auto AddNativeTag = [](FGameplayTag& OutTag, const TCHAR* TagName, const FString& TagComment)
	{
		OutTag = UGameplayTagsManager::Get().AddNativeGameplayTag(FName(TagName), TagComment);
	};

	AddNativeTag(GameplayTags.InputTag_LMB, TEXT("InputTag.LMB"), TEXT("鼠标左键"));
	AddNativeTag(GameplayTags.InputTag_RMB, TEXT("InputTag.RMB"), TEXT("鼠标右键"));
	AddNativeTag(GameplayTags.InputTag_1, TEXT("InputTag.1"), TEXT("数字键1"));
	AddNativeTag(GameplayTags.InputTag_2, TEXT("InputTag.2"), TEXT("数字键2"));
	AddNativeTag(GameplayTags.InputTag_3, TEXT("InputTag.3"), TEXT("数字键3"));
	AddNativeTag(GameplayTags.InputTag_4, TEXT("InputTag.4"), TEXT("数字键4"));
}