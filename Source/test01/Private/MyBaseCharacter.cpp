// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseCharacter.h"
// 构造函数：创建绑定到右手骨骼插槽的武器子对象
AMyBaseCharacter::AMyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;   // 允许每帧 Tick（便于后续扩展）

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// 把武器挂到骨骼网格的"右手握把"插槽上
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);   // 武器本身不参与碰撞
}
void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	// 目前暂无额外逻辑（子类可覆写扩展）

}

UAbilitySystemComponent* AMyBaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}
