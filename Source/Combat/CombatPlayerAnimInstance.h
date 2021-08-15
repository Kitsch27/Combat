// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UDS_Character.h"
#include "Animation/AnimInstance.h"
#include "CombatPlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UCombatPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	FVector Velocity;
	
	/** 移动速率 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	float MoveSpeed = 0;

	/** 向前移动速率 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	float MoveForwardSpeed = 0;

	/** 向右移动速率 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	float MoveRightSpeed = 0;

	/** 是否在空中 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	bool bIsInAir = false;

	/** 是否在地面 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	bool bIsOnGround = true;

	/** 锁定状态 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	bool bIsLocking = false;

	/** 锁定状态 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	bool bIsGetHit = false;

	/** 人物状态 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Generic")
	FCharacterState CharacterState;
};
