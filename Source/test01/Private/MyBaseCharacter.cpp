// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
// 构造函数：创建绑定到右手骨骼插槽的武器子对象
AMyBaseCharacter::AMyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true; // 允许每帧 Tick（便于后续扩展）

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// 把武器挂到骨骼网格的"右手握把"插槽上
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 武器本身不参与碰撞
}

void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 目前暂无额外逻辑（子类可覆写扩展）
}

// 实现 IAbilitySystemInterface：返回本角色绑定好的 GAS 组件（供系统/技能直接获取）
UAbilitySystemComponent* AMyBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

FVector AMyBaseCharacter::GetCombatSocketLocation()
{
	return Weapon->GetSocketLocation(WeaponTipSocketName);
}

// 初始化角色的基础属性：把"初始属性 GE"施加到自身，一次性写入生命/法力等默认值
// 通常在 BeginPlay 或组件就绪后调用一次
void AMyBaseCharacter::InitializePrimaryAttributes() const
{
	// 1. 为该角色创建一个效果上下文（记录来源/施法者信息）
	const FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	// 2. 用 DefaultPrimaryAttributes 指定的 GE 生成一个 1 级的 OutgoingSpec（还没生效，只是规格）
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		DefaultPrimaryAttributes, 1.f, ContextHandle);
	// 3. 把该 Spec 应用到自己身上，触发属性初始写入（需判空 Data，避免异常）
	if (SpecHandle.IsValid())
	{
		GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(),
		                                                             GetAbilitySystemComponent());
	}
}

void AMyBaseCharacter::AddCharacterAbilities() const
{
	// 把通用 GAS 组件强转成项目自有类型（需要其中的自定义技能接口）
	UMyAbilitySystemComponent* MyASC = CastChecked<UMyAbilitySystemComponent>(AbilitySystem);
	if (!HasAuthority()) return;   // 技能授予只在服务器端执行，客户端直接跳过

	// 调用 AS 组件接口，把 StartupAbilities 配置的技能批量授予本角色
	MyASC->AddCharacterAbilities(StartupAbilities);
}
