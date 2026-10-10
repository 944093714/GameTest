// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/MyEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/MyAttributeSet.h"
#include "Components/SphereComponent.h"

// Sets default values
AMyEffectActor::AMyEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
	
}

void AMyEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* AscInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		//改为使用gameplay effect
		const UMyAttributeSet* MyAttribute = Cast<UMyAttributeSet>(AscInterface->GetAbilitySystemComponent()->GetAttributeSet(UMyAttributeSet::StaticClass()));
		UMyAttributeSet* MutableMyAttribute= const_cast<UMyAttributeSet*>(MyAttribute);
		MutableMyAttribute->SetHealth(MyAttribute->GetHealth()+25.f);
		Destroy();
	}
}

void AMyEffectActor::EndOverlap(UPrimitiveComponent* OnComponentEndOverlap, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
	
}

// Called when the game starts or when spawned
void AMyEffectActor::BeginPlay()
{
	Super::BeginPlay();
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AMyEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&AMyEffectActor::EndOverlap);
}



