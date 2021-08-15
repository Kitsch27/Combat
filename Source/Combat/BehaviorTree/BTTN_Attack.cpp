// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTN_Attack.h"
#include "AIController.h"
#include "Combat/Character/CombatEnemy.h"


UBTTN_Attack::UBTTN_Attack()
{
	
}

EBTNodeResult::Type UBTTN_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("UBTTN_Attack"))
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	ACombatEnemy* Enemy = Cast<ACombatEnemy>(OwnerController->GetPawn());
	if (Enemy == nullptr || AttackMontageArr.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTN_Attack:failed"))
		return EBTNodeResult::Failed;
	}

	// 随机选择攻击
	UAnimMontage* AttackMontage = AttackMontageArr[rand() % AttackMontageArr.Num()];
		
	if (AttackMontage == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UBTTN_Attack:failed, AttackMontage == nullptr"))
		return EBTNodeResult::Failed;  
	}

	Enemy->Attack(AttackMontage);
	UE_LOG(LogTemp, Log, TEXT("UBTTN_Attack:Succeeded"))
	return EBTNodeResult::Succeeded;
}
