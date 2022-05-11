// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foggy.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Attack2.generated.h"

/**
 * 
 */
UCLASS()
class FOGGY_API UBTTask_Attack2 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack2();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;

};