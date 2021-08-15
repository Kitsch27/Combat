// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetTargetDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTS_GetTargetDistance::UBTS_GetTargetDistance()
{
	
}

void UBTS_GetTargetDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Aic = OwnerComp.GetAIOwner();
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		AActor* Target = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (Target)
		{
			const float Distance = Aic->GetPawn()->GetDistanceTo(Target);
			BlackboardComp->SetValueAsFloat("TargetDistance", Distance);
		}
	}
	
}