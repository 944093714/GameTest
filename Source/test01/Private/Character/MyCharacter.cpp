// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyCharacter.h"

#include "AbilitySystemComponent.h"
#include "MyPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"

// 构造函数：设置角色的移动表现（朝向随速度旋转、锁定在平面内）
AMyCharacter::AMyCharacter()
{
	UE_LOG(LogTemp, Warning, TEXT("AMyCharacter构造函数"));
	// 让角色网格朝向当前移动方向（而非单纯跟随控制器朝向）
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // 绕Z轴转向速率 400°/s
	// 把角色运动约束在平面内（适合俯视角/地图平面游戏），出生时吸附到平面
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 不使用控制器的 Pitch/Roll/Yaw 旋转（由 bOrientRotationToMovement 接管朝向）
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll  = false;
	bUseControllerRotationYaw   = false;
}

// 服务器端：玩家控制器接管（possess）此角色时被调用 → 初始化 GAS 信息
void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	SetPlayerInfo();   // 服务器端初始化
}

// 客户端：PlayerState 在客户端被复制并到达后触发 → 同样初始化 GAS 信息
void AMyCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	SetPlayerInfo();   // 客户端初始化
}

// 把本角色外链到 PlayerState 中创建的 GAS 组件与属性集，并完成 ActorInfo 绑定
void AMyCharacter::SetPlayerInfo()
{
	AMyPlayerState* MyPlayerState = GetPlayerState<AMyPlayerState>();
	// 告诉 GAS 组件"占有者是玩家状态、对应的演练者是本角色"，GAS 才能开始工作
	MyPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(MyPlayerState, this);

	// 从 PlayerState 取回 GAS 组件与属性集，供本角色后续使用（存进基类的指针）
	AbilitySystem = MyPlayerState->GetAbilitySystemComponent();
	AttributeSet  = MyPlayerState->GetAttributeSet();
}
