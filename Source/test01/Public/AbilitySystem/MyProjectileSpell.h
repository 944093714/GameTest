// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/MyGameplayAbility.h"
#include "MyProjectileSpell.generated.h"

class AMyProjectile;
/**
 * 
 */
UCLASS()
class TEST01_API UMyProjectileSpell : public UMyGameplayAbility
{
	GENERATED_BODY()
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	TSubclassOf<AMyProjectile> ProjectileClass;
	
	
};
