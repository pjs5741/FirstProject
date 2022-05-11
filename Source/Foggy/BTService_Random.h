// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foggy.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Random.generated.h"

/**
 * 
 */
UCLASS()
class FOGGY_API UBTService_Random : public UBTService
{
	GENERATED_BODY()
		UBTService_Random();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};