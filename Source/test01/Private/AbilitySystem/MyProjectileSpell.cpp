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
	const bool bIsServer = HasAuthority(&ActivationInfo);
	if (!bIsServer) return;

	ICombatInterface* CombatInerface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (CombatInerface)
	{
		const FVector SocketLocation = CombatInerface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		
		//TODO：添加旋转
		
		
		AMyProjectile* Projectile = GetWorld()->SpawnActorDeferred<AMyProjectile>(
			ProjectileClass, SpawnTransform, GetOwningActorFromActorInfo(), Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		//TODO: 添加一个GE效果
		
		
		Projectile->FinishSpawning(SpawnTransform);
		
	}
}
