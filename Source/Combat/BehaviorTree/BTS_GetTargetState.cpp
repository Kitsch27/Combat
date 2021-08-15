// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_GetTargetState.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/Character/CombatCharacter.h"


void UBTS_GetTargetState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		ACombatCharacter* Target = Cast<ACombatCharacter>(BlackboardComp->GetValueAsObject("TargetActor"));
		if (Target)
		{
			BlackboardComp->SetValueAsBool("bTargetIsDead", Target->GetCharacterState().bIsDead);
		}

		AAIController* AIController = OwnerComp.GetAIOwner();
		ACombatCharacter* Self = Cast<ACombatCharacter>(AIController->GetPawn());
		if (Self !=  nullptr)
		{
			BlackboardComp->SetValueAsBool("bIsDead", Self->GetCharacterState().bIsDead);
		}
	}
}
