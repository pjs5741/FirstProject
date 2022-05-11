// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Random.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BossCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_Random::UBTService_Random()
{
	NodeName = TEXT("Result");
	Interval = 1.0f;

	bNotifyBecomeRelevant = true;
}

void UBTService_Random::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	int x = 1;
	x = FMath::RandRange(1, 10);



	if (x <= 7)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMyAIController::Result, 1);
	}
	else if (x <= 10)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsInt(AMyAIController::Result, 2);
	}
	

}