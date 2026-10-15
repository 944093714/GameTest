// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyProjectileSpell.h"

#include "Actor/MyProjectile.h"
#include "GameFramework/PlayerController.h"
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
//==================
		// —— 旋转：让火焰弹朝向鼠标 ——
		// 施法者 AvatarActor(能力不是 Actor，方向需从 AvatarActor 身上取)
		AActor* AvatarActor = GetAvatarActorFromActorInfo();

		// 默认朝向：若拿不到鼠标点(无控制器/未命中)，则朝角色当前前向发射
		FVector TargetLocation = SocketLocation;
		if (AvatarActor)
		{
			TargetLocation = SocketLocation + AvatarActor->GetActorForwardVector() * 100.f;
		}

		// 尝试读取鼠标下的可见命中点作为真实目标
		if (const APlayerController* PC = Cast<APlayerController>(AvatarActor->GetInstigatorController()))
		{
			FHitResult CursorHit;
			if (PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit) && CursorHit.bBlockingHit)
			{
				TargetLocation = CursorHit.ImpactPoint;   // 鼠标下的可见命中点(通常投影到地面/物体)
			}
		}

		// 由"施法点 -> 鼠标点"的方向构造旋转(投射物沿本地 X 轴飞行)
		SpawnTransform.SetRotation((TargetLocation - SocketLocation).Rotation().Quaternion());
//=================

		// 延迟生成子弹：先配置好子弹的 Owner 与 Instigator，再在 FinishSpawning 时真正产出
		AMyProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMyProjectile>(
			ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO: 添加一个GE效果


		// 完成延迟生成，触发子弹的事件(如 OnConstruction)
		Projectile->FinishSpawning(SpawnTransform);
		// 结束位置，文件末尾新增
		EndAbility(Handle, ActorInfo, ActivationInfo, true, /*正常结束，非取消*/ false);

	}
}
