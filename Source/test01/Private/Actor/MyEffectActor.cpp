// Fill out your copyright notice in the Description page of Project Settings.


// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Components/SphereComponent.h"

// 构造函数：搭建"静态网格根组件 + 球形触发区"的结构
AMyEffectActor::AMyEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;   // 不需要每帧 Tick（事件驱动）

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);                  // 静态网格作为根组件

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());   // 球体挂在根组件下作为触发区
}

// 重叠开始：有 Actor 进入球形触发区时调用
void AMyEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 仅对"实现了 GAS 接口"的 Actor（玩家/敌人）生效
	if (IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		// TODO(注释)：这里正改为使用 GameplayEffect（目前是临时直接改属性）
		// 从对方 GAS 组件取回本项目的属性集
		const UMyAttributeSet* MyAttribute = Cast<UMyAttributeSet>(
			AscInterface->GetAbilitySystemComponent()->GetAttributeSet(UMyAttributeSet::StaticClass()));
		// GAS 惯例：属性变更需通过可变指针操作，故去掉 const
		UMyAttributeSet* MutableMyAttribute = const_cast<UMyAttributeSet*>(MyAttribute);
		// 直接加 25 点生命（临时实现，正式版应走 GameplayEffect）
		MutableMyAttribute->SetHealth(MyAttribute->GetHealth() + 25.f);
		Destroy();   // 效果用一次后销毁自身
	}
}

// 重叠结束：有 Actor 离开球体时调用（当前为空，暂无逻辑）
void AMyEffectActor::EndOverlap(UPrimitiveComponent* OnComponentEndOverlap, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

// 游戏开始时：把重叠进入/离开回调动态绑定到球体组件的委托上
void AMyEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AMyEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AMyEffectActor::EndOverlap);
}



