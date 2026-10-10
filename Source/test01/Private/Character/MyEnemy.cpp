// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"

// 构造函数：为敌人创建自己的 GAS 组件与属性集（敌人不经过 PlayerState）
AMyEnemy::AMyEnemy()
{
	// 敌人自己拥有 GAS 组件
	AbilitySystem = CreateDefaultSubobject<UMyAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetIsReplicated(true);
	// 使用 Minimal 复制模式（只复制最低限度的 GE 效果给远程客户端，节省带宽）
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	// 敌人自己的属性集（生命/法力等）
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>("MyAttributeSet");
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	// 敌人既是 Owner 也是 AvatarActor（Owner=拥有者，此角色=演练者）
	AbilitySystem->InitAbilityActorInfo(this, this);
}

// 鼠标悬停到敌人上：高亮
void AMyEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("AMyEnemy::HighlightActor"));
	IsHighlighted = true;
}

// 鼠标移开敌人：取消高亮
void AMyEnemy::UnHighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("AMyEnemy::UnHighlightActor"));
	IsHighlighted = false;
}



