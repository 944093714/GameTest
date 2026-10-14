// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/MyAttributeSet.h"
#include "GameFramework/Character.h"   // ACharacter 完整类型(做 Cast<ACharacter> 时要访问其成员, 必须包含)
#include "GameplayEffect.h"   // FGameplayEffectModCallbackData 等回调类型定义于此, 不包含则类型不完整无法访问成员
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"


// 各属性复制到达时，用 GAMEPLAYATTRIBUTE_REPNOTIFY 把 GAS 内部缓存自动同步到新值
void UMyAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Health, OldHealth);
}

void UMyAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& MaxOldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxHealth, MaxOldHealth);
}

void UMyAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMna) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Mana, OldMna);
}

void UMyAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& MaxOldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, MaxMana, MaxOldMana);
}

void UMyAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UMyAttributeSet, Strength, OldStrength);
}

// 默认构造函数定义：成员已在头文件里给了默认值(= nullptr 等)，这里只需空实现让链接通过
FEffectProperties::FEffectProperties()
{
}

// 从效果回调数据中提取"来源方"与"目标方"的关键信息，填充到 Props 中
// 用途：把 GAS 底层的 TArray(Data) 打散成易用的结构体(来源/目标 的 ASC、Actor、Controller、Character)
void UMyAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,FEffectProperties& Props) const
{
	// —— 来源方(施法者)：从效果上下文向上追溯到原始施法者的 ASC ——
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// 来源 ASC 已初始化并有效时，再取出其 AvatarActor 与 PlayerController
	if (IsValid(Props.SourceASC) && Props.SourceASC->AbilityActorInfo.IsValid() && Props.SourceASC->AbilityActorInfo->AvatarActor.IsValid())
	{
		Props.SourceAvatarActor = Props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		Props.SourceController = Props.SourceASC->AbilityActorInfo->PlayerController.Get();

		// 有些来源(如子类/召唤物)没有直接控制器，退回从 Pawn 身上反取控制器
		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}

		// 有控制器则进一步拿到来源角色
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	// —— 目标方(受影响者)：直接从 Data.Target 读取 AvatarActor / Controller ——
	if (Data.Target.AbilityActorInfo.IsValid())
	{
		Props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		Props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

// 构造函数：在此给属性设初始值（Strength 这类主动型属性默认从 0 开始，由等级/GE 后续装配）
UMyAttributeSet::UMyAttributeSet()
{
	InitHealth(50.f);
	InitMaxHealth(100.f);
	InitMana(50.f);
	InitMaxMana(100.f);
}

// 声明可复制属性：无条件复制，任何变化都收到通知(REPNOTIFY_Always)
void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 副本类属性(血/蓝，直接决定生存)，变化实时同步
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
	// 主属性(力量等)，同样参与复制，供战斗计算使用
	DOREPLIFETIME_CONDITION_NOTIFY(UMyAttributeSet, Strength, COND_None, REPNOTIFY_Always);
}

// 属性变化前回调：把即将写入的新值限制在合理范围内，防止血/蓝越界(负数、超过上限)
//预属性调用，在属性值真正写回之前触发，可以正确的限定我们需要的值，以及可以可以对数值做自己想要的效果
void UMyAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// 生命值：只允许落在区间 [0, MaxHealth]
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	// 法力：只允许落在区间 [0, MaxMana]
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	// 最大生命：下限限制为非负，避免上限被扣成负数导致血条反向
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
	// 最大法力：同上
	else if (Attribute == GetMaxManaAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}


// 后置游戏效果执行：效果已真正写入属性,拿到了最终的数值结果,在这里做二次收尾与派生逻辑
void UMyAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	
	FEffectProperties Props;
	SetEffectProperties(Data, Props);
	
	// FGameplayEffectContextHandle EffectContextHandle = Data.EffectSpec.GetContext();
	// UAbilitySystemComponent* SourceASC = EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();
	//
	// if (IsValid(SourceASC) && SourceASC->AbilityActorInfo.IsValid() && SourceASC->AbilityActorInfo->AvatarActor.
	// 	IsValid())
	// {
	// 	AActor* SourcAvatarActor = SourceASC->AbilityActorInfo->AvatarActor.Get();
	// 	const AController* Sourcecontroller = SourceASC->AbilityActorInfo->PlayerController.Get();
	// 	if (Sourcecontroller == nullptr && SourcAvatarActor != nullptr)
	// 	{
	// 		if (const APawn* Pawn = Cast<APawn>(Sourcecontroller))
	// 		{
	// 			Sourcecontroller = Pawn->GetController();
	// 		}
	// 	}
	//
	// 	if (Sourcecontroller)
	// 	{
	// 		ACharacter* SourceACharacter = Cast<ACharacter>(Sourcecontroller->GetPawn());
	// 	}
	// }
	//
	// if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo.IsValid())
	// {
	// 	AActor* TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
	// 	AController* TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
	// 	ACharacter* Targetcharacter = Cast<ACharacter>(TargetAvatarActor);
	// 	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
	// 		TargetAvatarActor);
	// }
}
