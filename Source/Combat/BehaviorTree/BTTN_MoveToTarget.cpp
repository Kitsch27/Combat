// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTN_MoveToTarget.h"


#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/Character/CombatCharacter.h"
#include "Combat/Character/CombatEnemy.h"
#include "Combat/Character/CombatPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTN_MoveToTarget::UBTTN_MoveToTarget()
{
	
}

EBTNodeResult::Type UBTTN_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("MoveToTarget"));
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		ACombatCharacter* Target = Cast<ACombatCharacter>(BlackboardComp->GetValueAsObject("TargetActor"));
		
		if (!IsValid(Target))
		{
			ACombatPlayer* Player = Cast<ACombatPlayer>(Target);
			if (!IsValid(Player))
			{
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Failed:!Player"));
				return EBTNodeResult::Failed;
			}
		}

		AAIController* OwnerController = OwnerComp.GetAIOwner();
		ACombatEnemy* Owner = Cast<ACombatEnemy>(OwnerController->GetPawn());
		if(!OwnerController || !Owner)
		{
			return EBTNodeResult::Failed;
		}
		
		UCharacterMovementComponent* MovementCom = Owner->GetCharacterMovement();
		MovementCom->MaxWalkSpeed = MoveSpeed;
		OwnerController->SetFocus(Target);
		if (OwnerController->MoveToActor(Target, AcceptanceRadius) == EPathFollowingRequestResult::RequestSuccessful)
		{
			return EBTNodeResult::Succeeded;
		}
		return EBTNodeResult::Failed;
	}
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Red, TEXT("Failed:!BlackboardComp"));

	// 失败返回
	return EBTNodeResult::Failed;
}
