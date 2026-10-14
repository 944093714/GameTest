// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"

// 便捷宏：为每个属性自动生成「属性获取器 / 值获取 / 值设置 / 值初始化」四类访问函数
// 展开后得到：Get{Property}(ASC名)、Get{Property}Attribute()、Set{Property}(新值)、Init{Property}(初值)
// 例如本类的 Strength 属性通过 ATTRIBUTE_ACCESSORS(UMyAttributeSet, Strength) 生成
//   GetStrength() / GetStrengthAttribute() / SetStrength(Value) / InitStrength(Value) 四个函数。
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// 效果回调的属性提取结果：把一次 GE 变化涉及到的「来源方」与「目标方」信息打包存放
// 由 SetEffectProperties() 填充后交给后续逻辑(伤害计算/表现)统一使用
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()
	FEffectProperties();

	// 本次效果上下文（可追溯原始施法者的 ASC）
	FGameplayEffectContextHandle EffectContextHandle;

	// —— 来源方(施法者) ——
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;   // 施法者能力组件
	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;            // 施法者外观/载体 Actor
	UPROPERTY()
	AController* SourceController = nullptr;        // 施法者控制器
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;          // 施法者角色(若有)

	// —— 目标方(受影响者) ——
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;   // 目标能力组件
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;            // 目标外观/载体 Actor
	UPROPERTY()
	AController* TargetController = nullptr;        // 目标控制器
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;          // 目标角色(若有)
};

/**
 * 属性集类：定义角色的「生命/最大生命/法力/最大法力」四项基础属性。
 * ReplicatedUsing = 属性被复制到客户端时触发对应 OnRep 回调以同步缓存。
 */
UCLASS()
class TEST01_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UMyAttributeSet(); // 构造函数：初始化四项属性的默认值
	// 网络复制相关：声明哪些属性参与复制，并指定复制条件与变化通知方式
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//前置游戏效果执行
	// 当某个属性要写入新值前触发。此时 GE 尚未真正修改属性，可在这里把 NewValue 约束到合法范围(如血/蓝不为负、不超上限)
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	//后置游戏效果执行
	// 效果已真正写入属性后触发。此时可拿到最终数值进行收尾与派生逻辑(如根据伤害结算换算、触发表现等)
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	// 力量（主属性，参与战斗/伤害计算；复制变化时触发 OnRep_Strength）
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Strength);
	
	// 生命值（Vital 关键属性，复制变化时触发 OnRep_Health）
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Health);

	// 最大生命
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxHealth);

	// 法力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category = "Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, Mana);

	// 最大法力
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category = "Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UMyAttributeSet, MaxMana);

	// 各属性的复制通知回调（收到新值时把 GAS 内部缓存同步到最新）
	// 均以 ReplicatedUsing 绑定到对应 UPROPERTY，客户端收到属性更新时自动调用；
	// 参数为旧的属性数据(未启用完整复制环境时仅作占位)，用于对比新旧值
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& MaxOldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMna) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& MaxOldMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	

private:
	// 从效果回调数据中提取来源/目标的 ASC、Actor、Controller、Character 等，填充到 Props
	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& Props) const;
};
