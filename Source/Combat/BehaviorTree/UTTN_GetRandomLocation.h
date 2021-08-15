// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UTTN_GetRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UTTN_GetRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	/** 范围 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 500.f;
};
