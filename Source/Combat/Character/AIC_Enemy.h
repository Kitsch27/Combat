// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CombatCharacter.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AISenseConfig_Sight.h"

#include "AIC_Enemy.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API AAIC_Enemy : public AAIController
{
	GENERATED_BODY()

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;
	
public:
	AAIC_Enemy();

	/** 行为组件 */
	UPROPERTY()
	UBehaviorTreeComponent* BehaviorComp;

	/** 黑板组件 */
	UPROPERTY()
	UBlackboardComponent* BlackboardComp;

	UFUNCTION()
	void SightTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/** AI感知组件 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|AIPerceptionCmp")
	UAIPerceptionComponent* AIPerceptionCmp;

	/** AI视觉配置 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|AIConfigSight")
	UAISenseConfig_Sight* AIConfigSight;

	/** 是否看见玩家 */
	bool bAISeePlayer = false;

	/** 行为树 */
	UPROPERTY(EditAnywhere, Category = General)
	class UBehaviorTree* BhTree;

	/** 黑板数据 */
	UPROPERTY(VisibleAnywhere, Category = General)
	class UBlackboardData* BlackBoardData;

	UPROPERTY()
	ACombatCharacter* SelfCharacter;

	/** 是否已装备 */
	bool bIsEquipped;
};
