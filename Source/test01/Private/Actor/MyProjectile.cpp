// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyProjectile.h"

#include "NavigationSystemTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// 构造函数：创建并配置子弹的碰撞组件与运动组件
AMyProjectile::AMyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // 不需要每帧 Tick（事件驱动）
	bReplicates = true;                     // 启用网络同步,使子弹在服务器/客户端间表现一致

	// 创建球体作为根组件,仅用于重叠查询,不做实体阻挡
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(Sphere);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);           // 只进行查询,不产生物理碰撞
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);               // 默认忽略所有通道
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic,ECR_Overlap); // 动态物体设为重叠响应
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);  // 静态物体设为重叠响应
	Sphere->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);         // Pawn(玩家/敌人)设为重叠响应

	// 创建投射物运动组件,配置恒定的飞行速度且不受重力影响
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 550.f;
	ProjectileMovement->MaxSpeed = 550.f;
	ProjectileMovement->ProjectileGravityScale = 0.f; // 重力缩放为 0,子弹沿直线飞行
}

// 游戏开始或生成时调用：把重叠回调绑定到球体组件
void AMyProjectile::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AMyProjectile::OnSphereOverlap );

}

// 球体重叠回调：子弹碰到目标时触发,具体命中逻辑在此处补充实现
void AMyProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

