// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBaseCharacter.h"
AMyBaseCharacter::AMyBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
void AMyBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
