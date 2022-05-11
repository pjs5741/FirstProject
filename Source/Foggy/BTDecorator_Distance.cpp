// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Distance.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_Distance::UBTDecorator_Distance()
{
	NodeName = TEXT("Distance>=1000");
}

bool UBTDecorator_Distance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return false;
	bResult = (Target->GetDistanceTo(ControllingPawn) >= 1000.0f);
	return bResult;

}

