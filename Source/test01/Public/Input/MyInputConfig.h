
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MyInputConfig.generated.h"


/**
 * 结构体：单个 InputAction 与其对应 GameplayTag 的映射条目
 * 一个 UInputAction（Enhanced Input 的输入动作）+ 一个 FGameplayTag 绑定为一条
 */
USTRUCT(BlueprintType)
struct FMyInputAction
{
	GENERATED_BODY()

	/** 暴露给编辑器配置的输入动作（只在默认值时编辑） */
	UPROPERTY(EditDefaultsOnly)
	class UInputAction* InputAction = nullptr;

	/** 与该输入动作对应的 GameplayTag（作为该输入的标识，供技能等系统匹配） */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

/**
 * UMyInputConfig
 * 数据资产（DataAsset）：集中定义“输入动作 ↔ GameplayTag”的映射表。
 * 在 GAS + Enhanced Input 中，用它把 Ability Tag 与具体按键解耦：
 * 技能系统只需关心 GameplayTag，而无需关心玩家按的是哪个键。
 */
UCLASS()
class TEST01_API UMyInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	/**
	 * 根据指定的 GameplayTag 查找对应的 UInputAction。
	 * @param InputTag			要查找的输入标签
	 * @param bLogNotFound		为 true 时，若找不到会输出错误日志
	 * @return 				找到的 UInputAction 指针；找不到且 bLogNotFound 时返回 nullptr
	 */
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag,bool bLogNotFound = false) const;

	/** 输入动作与游戏标签的映射数组（在数据资产内容路径下配置） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMyInputAction> MyInputActions;
};
