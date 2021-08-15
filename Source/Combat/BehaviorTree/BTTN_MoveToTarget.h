// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTN_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTN_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UBTTN_MoveToTarget();

	/** 距离目标可停下的范围 */
	UPROPERTY(EditAnywhere)
	float AcceptanceRadius = -1;
	
	/** 移动速度 */
	UPROPERTY(EditAnywhere)
	float MoveSpeed = 600.f;
};

