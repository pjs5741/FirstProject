// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Distance.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_Distance::UBTService_Distance()
{
	NodeName = TEXT("Distance");
	Interval = 1.0f;
}

void UBTService_Distance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	auto Bosscharacter = Cast<ABossCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	bool bResult;

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));

	if (nullptr == Target)
		return;

	bResult = (Target->GetDistanceTo(ControllingPawn) >= 1000.0f);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("Distance"), bResult);
}
