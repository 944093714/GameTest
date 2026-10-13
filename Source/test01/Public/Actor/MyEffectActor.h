// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEffectActor.generated.h"

class UGameplayEffect;

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

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor,TSubclassOf<UGameplayEffect> Effect);
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> MyEffect;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Applied Effects")
	TSubclassOf<UGameplayEffect> MyTimeEffect;
};
