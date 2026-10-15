// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyProjectileSpell.h"

#include "Actor/MyProjectile.h"
#include "Interface/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UMyProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                         const FGameplayAbilityActorInfo* ActorInfo,
                                         const FGameplayAbilityActivationInfo ActivationInfo,
                                         const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	//UKismetSystemLibrary::PrintString(this,FString("ActivateAbility(C++)"),true,true,FLinearColor::Green,3.f);
	// 仅服务器拥有生成本币的权限，客户端直接返回
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	// 从施法者身上取战斗接口，用于获取子弹的出生位置(战斗插槽)
	ICombatInterface* CombatInerface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInerface)
	{
		// 取插槽位置(如武器/手部挂点)作为子弹生成的世界坐标
		const FVector SocketLocation = CombatInerface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);

		//TODO：添加旋转


		// 延迟生成子弹：先配置好子弹的 Owner 与 Instigator，再在 FinishSpawning 时真正产出
		AMyProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMyProjectile>(
			ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: 添加一个GE效果


		// 完成延迟生成，触发子弹的事件(如 OnConstruction)
		Projectile->FinishSpawning(SpawnTransform);

	}
}
