// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyInputConfig.generated.h"
#include "GameplayTagContainer.h"


USTRUCT(BlueprintType)
struct FMyInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	const class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	const FGameplayTag InputTag = FGameplayTag();
};

/**
 * 
 */
UCLASS()
class TEST01_API UMyInputConfig : public UDataAsset
{
	GENERATED_BODY()
};
