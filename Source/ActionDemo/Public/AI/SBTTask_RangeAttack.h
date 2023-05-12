// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONDEMO_API USBTTask_RangeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USBTTask_RangeAttack();

protected:
	UPROPERTY(EditAnywhere, Category="AI")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, Category="AI")
	float MaxProjSpread;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
