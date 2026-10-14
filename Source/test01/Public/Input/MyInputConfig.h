
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "MyInputConfig.generated.h"


USTRUCT(BlueprintType)
struct FMyInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	class UInputAction* InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

/**
 * 
 */
UCLASS()
class TEST01_API UMyInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag,bool bLogNotFound = false) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FMyInputAction> MyInputActions;
};
