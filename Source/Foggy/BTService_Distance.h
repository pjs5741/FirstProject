// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Foggy.h"
#include "BehaviorTree/BTService.h"
#include "BTService_Distance.generated.h"

/**
 * 
 */
UCLASS()
class FOGGY_API UBTService_Distance : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_Distance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
