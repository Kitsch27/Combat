// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTN_FindWay.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTTN_FindWay : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	/** 移动范围 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 500.f;

	/** 移动速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 200.f;
	
};
