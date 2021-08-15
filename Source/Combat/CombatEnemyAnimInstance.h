// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UDS_Character.h"
#include "Animation/AnimInstance.h"
#include "CombatEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	FVector Velocity;
	
	/** 向前移动速率 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	float MoveForwardSpeed = 0;

	/** 向右移动速率 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	float MoveRightSpeed = 0;

	/** 人物状态 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	FCharacterState CharacterState;
};
