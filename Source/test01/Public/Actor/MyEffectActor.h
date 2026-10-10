// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEffectActor.generated.h"

class USphereComponent;

/**
 * 区域效果 Actor（暂为临时实现，见 OnOverlap 内的 TODO）
 * 由静态网格 + 球形触发区组成；当实现了 GAS 接口的角色进入球形重叠区时触发一次效果。
 * 当前直接修改属性值（加血），后续应改为使用 GameplayEffect。
 */
UCLASS()
class TEST01_API AMyEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AMyEffectActor();

	// 重叠开始回调：有 Actor 进入球体时触发
	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 重叠结束回调：有 Actor 离开球体时触发
	UFUNCTION()
	virtual void EndOverlap(UPrimitiveComponent* OnComponentEndOverlap, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;        // 球形重叠触发区（用于检测进入的角色）

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;      // 静态网格（作为可视化外观 / 根组件）

};
