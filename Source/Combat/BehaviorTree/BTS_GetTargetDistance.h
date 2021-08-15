// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_GetTargetDistance.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UBTS_GetTargetDistance : public UBTService
{
	GENERATED_BODY()
public:
	UBTS_GetTargetDistance();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
