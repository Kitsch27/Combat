// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Enemy.h"

#include "CombatPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"


AAIC_Enemy::AAIC_Enemy()
{
	PrimaryActorTick.bCanEverTick = true;

	// 行为组件
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	// 黑板组件
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	// AI感知组件
	AIPerceptionCmp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionCmp"));
	// AI视觉配置
	AIConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AIConfigSight"));

	if (AIPerceptionCmp)
	{
		// 设置基本视觉配置
		AIConfigSight->SightRadius = 1000.0f;
		AIConfigSight->LoseSightRadius = 2000.0f;
		AIConfigSight->PeripheralVisionAngleDegrees = 90.0f;
		AIConfigSight->DetectionByAffiliation.bDetectEnemies = true;
		AIConfigSight->DetectionByAffiliation.bDetectFriendlies = true;
		AIConfigSight->DetectionByAffiliation.bDetectNeutrals = true;

		AIPerceptionCmp->ConfigureSense(*AIConfigSight);
		AIPerceptionCmp->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	}
}


void AAIC_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	SelfCharacter = Cast<ACombatCharacter>(InPawn);
	if (IsValid(BehaviorComp))
	{
		BlackBoardData = BhTree->GetBlackboardAsset();
		BlackboardComp->InitializeBlackboard(*BlackBoardData);
		BlackboardComp->SetValueAsObject("SelfActor", InPawn);
		BehaviorComp->StartTree(*BhTree);
	}

	AIPerceptionCmp->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_Enemy::SightTargetPerceptionUpdated);
}

void AAIC_Enemy::OnUnPossess()
{
	Super::OnUnPossess();
	if (BehaviorComp != nullptr)
	{
		// 停止运行行为树
		BehaviorComp->StopTree(EBTStopMode::Safe);
	}
}

void AAIC_Enemy::SightTargetPerceptionUpdated(AActor* Actor, const FAIStimulus Stimulus)
{
	ACombatPlayer* Player = Cast<ACombatPlayer>(Actor);
	if (IsValid(Player) && IsValid(SelfCharacter) && IsValid(BehaviorComp))
	{
		bAISeePlayer = Stimulus.WasSuccessfullySensed();

		// 是否能看见目标
		BlackboardComp->SetValueAsBool("CanSeeTarget", bAISeePlayer);
		if (bAISeePlayer)
		{
			// 获取对象位置
			const FVector TargetLocation = Player->GetActorLocation();
			BlackboardComp->SetValueAsVector("LastTargetLocation", TargetLocation);
			BlackboardComp->SetValueAsObject("TargetActor", Player);
		}
		
	}
	if (bAISeePlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("SightTargetPerceptionUpdated: Yes"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SightTargetPerceptionUpdated: No"));
	}
}
