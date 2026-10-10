// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyAttributeSet.h"

#include "Net/UnrealNetwork.h"

void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,Health,OldHealth);
}

void UMyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& MaxOldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,MaxHealth,MaxOldHealth);
}

void UMyAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMna) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,Health,OldMna);
}

void UMyAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& MaxOldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet,Health,MaxOldMana);
}

UMyAttributeSet::UMyAttributeSet()
{
	InitHealth(100.f);
	InitMaxHealth(100.f);
	InitMana(100.f);
	InitMaxMana(100.f);
}

void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY( UMyAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY( UMyAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY( UMyAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY( UMyAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);
}
