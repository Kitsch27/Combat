// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTN_Attack.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTN_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
	UBTTN_Attack();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> AttackMontageArr;

};
