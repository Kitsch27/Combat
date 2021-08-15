// Fill out your copyright notice in the Description page of Project Settings.


#include "UTTN_GetRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/Character/CombatEnemy.h"


 EBTNodeResult::Type UTTN_GetRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("UBTS_GetLocation"));
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("BlackboardComp == nullptr"));
		return EBTNodeResult::Failed;
	}
	
 	AAIController* OwnerController = OwnerComp.GetAIOwner();
 	APawn* Pawn = OwnerController->GetPawn();
 	
 	FNavLocation ResultLocation;
		
 	// 导航
 	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(OwnerController);
 	if(NavSystem && NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, ResultLocation))
 	{
 		UE_LOG(LogTemp, Log, TEXT("Update RandomLocationn"));
 		BlackboardComp->SetValueAsVector("RandomLocation", ResultLocation.Location);
 		return EBTNodeResult::Succeeded; 
 	}
 
 	// UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
 	// if (BlackboardComp)
 	// {
 	// 	AAIController* OwnerController = OwnerComp.GetAIOwner();
 	// 	ACombatEnemy* Owner = Cast<ACombatEnemy>(OwnerController->GetPawn());
 	// 	if(!OwnerController || !Owner)
 	// 	{
 	// 		return EBTNodeResult::Failed;
 	// 	}
	 //
 	// 	const FVector OriginLocation =  Owner->GetActorLocation();
 	// 	FNavLocation ResultLocation;
		//
 	// 	// 导航
 	// 	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(OwnerController);
	 //
 	// 	if(NavSystem && NavSystem->GetRandomReachablePointInRadius(OriginLocation, Radius, ResultLocation))
 	// 	{
 	// 		BlackboardComp->SetValueAsVector("RandomLocation", ResultLocation.Location);
 	// 	}
 	// }
	 //
 	// UE_LOG(LogTemp, Log, TEXT("UBTS_GetLocation Failed"));
 	return EBTNodeResult::Failed; 
}
