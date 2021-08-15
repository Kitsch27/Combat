// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_RotateToFaceBBEntry.h"
#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTN_SetFocus.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTN_SetFocus : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
