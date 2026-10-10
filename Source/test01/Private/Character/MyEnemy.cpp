// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyEnemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"


AMyEnemy::AMyEnemy()
{
	AbilitySystem = CreateDefaultSubobject<UMyAbilitySystemComponent>("AbilitySystem");
	AbilitySystem->SetIsReplicated(true);
	AbilitySystem->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	
	AttributeSet = CreateDefaultSubobject<UMyAttributeSet>("MyAttributeSet");
}

void AMyEnemy::BeginPlay()
{
	Super::BeginPlay();
	AbilitySystem->InitAbilityActorInfo(this,this);
}


void AMyEnemy::HighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("AMyEnemy::HighlightActor"));
	IsHighlighted=true;
}


void AMyEnemy::UnHighlightActor()
{
	UE_LOG(LogTemp, Warning, TEXT("AMyEnemy::UnHighlightActor"));
	
	IsHighlighted=false;
}



