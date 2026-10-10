// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()
class TEST01_API AMyPlayerState : public APlayerState,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	
	AMyPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const{ return AttributeSet; }

protected:
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystem;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
