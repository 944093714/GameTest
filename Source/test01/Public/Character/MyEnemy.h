
#pragma once

#include "CoreMinimal.h"
#include "MyBaseCharacter.h"
#include "MyInterface.h"
#include "MyEnemy.generated.h"

/**
 * 
 */
UCLASS()
class TEST01_API AMyEnemy : public AMyBaseCharacter,public IMyInterface
{
	GENERATED_BODY()
public:
	//敌人接口
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//敌人接口结束
	
	AMyEnemy();
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsHighlighted;
	
	
};
