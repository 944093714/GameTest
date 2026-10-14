// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/MyAbilitySystemComponent.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Components/SphereComponent.h"

// 构造函数：搭建"静态网格根组件 + 球形触发区"的结构
AMyEffectActor::AMyEffectActor()
{
	PrimaryActorTick.bCanEverTick = false; // 不需要每帧 Tick（事件驱动）

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));

	//
	// Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	// SetRootComponent(Mesh);                  // 静态网格作为根组件
	//
	// Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	// Sphere->SetupAttachment(GetRootComponent());   // 球体挂在根组件下作为触发区
}

FActiveGameplayEffectHandle AMyEffectActor::ApplyEffectToTarget(AActor* TargetActor,
                                                                TSubclassOf<UGameplayEffect> Effect)
{
	// 通过 GAS 蓝图接口取目标的能力系统组件，拿不到(未实现接口)则直接返回空句柄
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	if (TargetAsc == nullptr) return FActiveGameplayEffectHandle();

	// 提前判空：蓝图未设置 Effect 时直接返回，避免空指针崩溃
	if (!Effect)
	{
		return FActiveGameplayEffectHandle();
	}

	// 创建效果上下文，并把"来源"标记为当前 Actor（之后的 IsValid 判断会用到）
	FGameplayEffectContextHandle EffectContextHandle = TargetAsc->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	FGameplayEffectSpecHandle EffectSpecHandle = TargetAsc->MakeOutgoingSpec(Effect, Levels, EffectContextHandle);

	FActiveGameplayEffectHandle ActiveHandle;
	// MakeOutgoingSpec 可能失败（Data 为空），须判空后再解引用
	if (EffectSpecHandle.IsValid())
	{
		// 真正把效果施加到目标身上，拿到唯一句柄供后续"取消效果"使用
		ActiveHandle = TargetAsc->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

		// 施加成功则把句柄按目标能力组件分组记录，方便按目标批量取消
		if (ActiveHandle.IsValid())
		{
			ActiveEffectHandles.FindOrAdd(TargetAsc).Handles.Add(ActiveHandle);
		}
	}

	return ActiveHandle;
}

// 取消某目标身上的效果：把之前施加的每个句柄都从目标上移除掉
void AMyEffectActor::RemoveEffectFromTarget(AActor* TargetActor)
{
	UAbilitySystemComponent* TargetAsc = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetAsc == nullptr) return;

	// 查找该目标是否确实被本 Actor 施加过效果
	if (FActiveEffectHandles* EffectHandles = ActiveEffectHandles.Find(TargetAsc))
	{
		for (const FActiveGameplayEffectHandle& Handle : EffectHandles->Handles)
		{
			// 通过句柄移除指定效果
			TargetAsc->RemoveActiveGameplayEffect(Handle);
		}
		// 移除完毕后清理对应记录，避免句柄残留
		ActiveEffectHandles.Remove(TargetAsc);
	}
}

// 重叠进入：有 Actor 进入触发区时，按"进入时施加"策略分别施加三种效果
void AMyEffectActor::OnOverlap(AActor* TargetActor)
{
	// 普通效果：策略为 ApplyOnOverlay 时才施加
	if (MyEffectPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, MyEffect);
	}

	// 限时效果
	if (MyTimeEffectPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, MyTimeEffect);
	}

	// 长效效果
	if (MyLongTimeEffectPolicy == EEffectApplicationPolicy::ApplyOnOverlay)
	{
		ApplyEffectToTarget(TargetActor, MyLongTimeEffect);
	}
}

// 重叠离开：有 Actor 离开触发区时，按移除策略取消效果，并按需销毁自身
void AMyEffectActor::OnEndOverlap(AActor* TargetActor)
{
	// 移除策略为"离开时移除"才取消施加在目标身上的效果
	if (RemovePolicy == EEffectRemovealPolicy::RemoveOnEndOverlap)
	{
		RemoveEffectFromTarget(TargetActor);
	}
}


// // 重叠开始：有 Actor 进入球形触发区时调用
// void AMyEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
// {
// 	// 仅对"实现了 GAS 接口"的 Actor（玩家/敌人）生效
// 	if (IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(OtherActor))
// 	{
// 		// TODO(注释)：这里正改为使用 GameplayEffect（目前是临时直接改属性）
// 		// 从对方 GAS 组件取回本项目的属性集
// 		const UMyAttributeSet* MyAttribute = Cast<UMyAttributeSet>(
// 			AscInterface->GetAbilitySystemComponent()->GetAttributeSet(UMyAttributeSet::StaticClass()));
// 		// GAS 惯例：属性变更需通过可变指针操作，故去掉 const
// 		UMyAttributeSet* MutableMyAttribute = const_cast<UMyAttributeSet*>(MyAttribute);
// 		// 直接加 25 点生命（临时实现，正式版应走 GameplayEffect）
// 		MutableMyAttribute->SetHealth(MyAttribute->GetHealth() + 10.f);
// 		// 直接加 20点蓝量（临时实现，正式版应走 GameplayEffect）
// 		MutableMyAttribute->SetMana(MyAttribute->GetMana() - 10.f);
// 		Destroy();   // 效果用一次后销毁自身
// 	}
// }
//
// // 重叠结束：有 Actor 离开球体时调用（当前为空，暂无逻辑）
// void AMyEffectActor::EndOverlap(UPrimitiveComponent* OnComponentEndOverlap, AActor* OtherActor,
// 	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
// {
// }
//
// // 游戏开始时：把重叠进入/离开回调动态绑定到球体组件的委托上
// void AMyEffectActor::BeginPlay()
// {
// 	Super::BeginPlay();
// 	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMyEffectActor::OnOverlap);
// 	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMyEffectActor::EndOverlap);
// }
