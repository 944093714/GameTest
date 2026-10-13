// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "MyEffectActor.generated.h"

class UGameplayEffect;

// 效果施加策略：决定 GameplayEffect 在何时被应用到目标身上
UENUM(BlueprintType)
enum class EEffectApplicationPolicy: uint8
{
	ApplyOnOverlay,     // 进入重叠区域时施加
	ApplyOnEndOverlay,  // 离开重叠区域时施加
	DoNotApply          // 不施加
};

// 效果移除策略：决定 GameplayEffect 在何时被移除
UENUM(BlueprintType)
enum class EEffectRemovealPolicy: uint8
{
	RemoveOnEndOverlap, // 离开重叠区域时移除
	DoNotRemove         // 不移除
};

// 一个目标身上施加的一组效果句柄容器
// UHT 不允许 TMap 的 value 直接使用 TArray，因此用 USTRUCT 包装一层
USTRUCT(BlueprintType)
struct FActiveEffectHandles
{
	GENERATED_BODY()

	// 该目标被施加的全部效果的句柄列表
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> Handles;
};

UCLASS()
class TEST01_API AMyEffectActor : public AActor
{
	GENERATED_BODY()

public:
	// 构造函数：搭建静态网格根组件 + 球形触发区的结构
	AMyEffectActor();

protected:
	virtual void BeginPlay() override;

	// 将指定的 GameplayEffect 施加到目标 Actor 上，返回效果句柄（用于之后取消）
	UFUNCTION(BlueprintCallable)
	FActiveGameplayEffectHandle ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> Effect);

	// 取消施加在目标 Actor 上的效果（遍历 ActiveEffectHandles 逐个移除）
	UFUNCTION(BlueprintCallable)
	void RemoveEffectFromTarget(AActor* TargetActor);

	// 重叠回调：有 Actor 进入触发区时调用
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	// 重叠结束回调：有 Actor 离开触发区时调用
	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	// 若为 true，在效果被移除时销毁自身
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Applied Effect")
	bool bDestoryOnEffectRemoval = false;

	// 普通效果：瞬时施加到目标
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> MyEffect;

	// 普通效果的施加策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy MyEffectPolicy = EEffectApplicationPolicy::DoNotApply;


	// 限时效果：附加持续时间的治疗效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> MyTimeEffect;

	// 限时效果的施加策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy MyTimeEffectPolicy = EEffectApplicationPolicy::DoNotApply;

	// 长效效果：长时间持续回复效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> MyLongTimeEffect;

	// 长效效果的施加策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy MyLongTimeEffectPolicy = EEffectApplicationPolicy::DoNotApply;

	// 效果移除策略
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectRemovealPolicy RemovePolicy = EEffectRemovealPolicy::RemoveOnEndOverlap;

	// 记录已施加到目标身上的效果句柄，按"目标能力组件"分组，供 RemoveEffectFromTarget 取消时使用
	UPROPERTY()
	TMap<TWeakObjectPtr<UAbilitySystemComponent>, FActiveEffectHandles> ActiveEffectHandles;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effect")
	float Levels = 1.f;
};
