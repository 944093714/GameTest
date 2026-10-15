// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
// 子弹类：由能力触发生成的投射物，负责飞行并对命中的目标进行碰撞检测
UCLASS()
class TEST01_API AMyProjectile : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数：创建球体碰撞组件并配置运动组件
	AMyProjectile();
	// 投射物运动组件：控制子弹的飞行速度、方向与重力
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
protected:
	// 游戏开始或生成时调用：这里绑定球体的重叠回调
	virtual void BeginPlay() override;
	// 球体重叠回调：子弹命中其他 Actor 时触发,用于后续处理命中逻辑
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult & SweepResult);


private:
	// 球形碰撞组件：作为子弹的碰撞体积,用于重叠检测
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
};
