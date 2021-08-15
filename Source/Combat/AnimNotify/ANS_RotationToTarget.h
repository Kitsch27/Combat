// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Combat/Character/CombatCharacter.h"

#include "ANS_RotationToTarget.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UANS_RotationToTarget : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;

	UPROPERTY()
	AActor* Target;

	UPROPERTY()
	ACombatCharacter* Self;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LockDistance = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotateSpeed = 10.f;
	
};
