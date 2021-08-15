// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTN_SetFocus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

UBTTN_SetFocus::UBTTN_SetFocus(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "SetFocus";
	// 只能选择Actor
	BlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTN_SetFocus, BlackboardKey), AActor::StaticClass());
}

EBTNodeResult::Type UBTTN_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning, TEXT("SetFocus"));
	AAIController* OwnerController = OwnerComp.GetAIOwner();
	if (OwnerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("OwnerController == nullptr"));
		return EBTNodeResult::Failed;
	}

	if (BlackboardKey.SelectedKeyType != UBlackboardKeyType_Object::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("SelectedKeyType != UBlackboardKeyType_Object"));
		return EBTNodeResult::Failed;
	}

	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	UObject* KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID());
	AActor* ActorValue = Cast<AActor>(KeyValue);

	if (ActorValue == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ActorValue == nullptr"));
		return EBTNodeResult::Failed;
	}

	
	if (ActorValue != OwnerController->GetPawn())
	{
		OwnerController->SetFocus(ActorValue);
		UE_LOG(LogTemp, Error, TEXT("SetFocus"));
	}
	else
	{
		OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
		UE_LOG(LogTemp, Error, TEXT("ClearFocus"));
	}
		

	UE_LOG(LogTemp, Warning, TEXT("SetFocus Succeeded"));
	return EBTNodeResult::Succeeded;
}
