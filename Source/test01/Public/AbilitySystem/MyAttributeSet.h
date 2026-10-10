// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "MyAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TEST01_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData Health;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UMyAttributeSet, Health);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UMyAttributeSet, MaxHealth);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData Mana;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UMyAttributeSet, Mana);
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UMyAttributeSet, MaxMana);
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& MaxOldHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMna) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& MaxOldMana) const;
	


};
