// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTN_FindWay.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Combat/Character/CombatEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"


EBTNodeResult::Type UBTTN_FindWay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("FindWay"));
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		AAIController* OwnerController = OwnerComp.GetAIOwner();
		ACombatEnemy* Owner = Cast<ACombatEnemy>(OwnerController->GetPawn());
		if(!OwnerController || !Owner)
		{
			return EBTNodeResult::Failed;
		}

		const FVector OriginLocation =  Owner->GetActorLocation();
		FNavLocation ResultLocation;
		
		// 导航
		UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(OwnerController);

		if(NavSystem && NavSystem->GetRandomReachablePointInRadius(OriginLocation, Radius, ResultLocation))
		{
			UCharacterMovementComponent* MovementCom = Owner->GetCharacterMovement();
			MovementCom->MaxWalkSpeed = WalkSpeed;

			UAIBlueprintHelperLibrary::CreateMoveToProxyObject(OwnerController, Owner, ResultLocation.Location);
			return EBTNodeResult::Succeeded;
		}
	}

	// 失败 返回
	return EBTNodeResult::Failed;
}
