// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "AbilitySystemComponent.h"
#include "MyPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyCharacter::AMyCharacter()
{
	UE_LOG(LogTemp, Warning,TEXT("AMyCharacter构造函数"));
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);//YZX
	GetCharacterMovement()->bConstrainToPlane= true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	
	bUseControllerRotationPitch=false;
	bUseControllerRotationRoll=false;
	bUseControllerRotationYaw=false;
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//使用给服务器
	SetPlayerInfo();
	
}

void AMyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//使用给客户端
	SetPlayerInfo();
}

void AMyCharacter::SetPlayerInfo()
{
	AMyPlayerState* MyPlayerState =  GetPlayerState<AMyPlayerState>();
	MyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MyPlayerState,this);
	
	AbilitySystem = MyPlayerState->GetAbilitySystemComponent();
	AttributeSet = MyPlayerState->GetAttributeSet();
}
