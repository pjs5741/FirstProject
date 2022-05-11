// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack3.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BossCharacter.h"

UBTTask_Attack3::UBTTask_Attack3()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack3::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Ressult = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyCharacter = Cast<ABossCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyCharacter)
		return EBTNodeResult::Failed;

	MyCharacter->Attack3();
	IsAttacking = true;
	MyCharacter->OnAttackEnd.AddLambda([this]()->void {
		IsAttacking = false;
		});



	return EBTNodeResult::InProgress;
}

void UBTTask_Attack3::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}