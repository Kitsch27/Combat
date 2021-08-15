// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SetGravityScale.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UAN_SetGravityScale : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/** 重力比例 */
	UPROPERTY(EditAnywhere)
	float GravityScaleRate = 1.f;

	/** Z轴速度 */
	UPROPERTY(EditAnywhere)
	bool bChangeZVelocity = true;
	/** Z轴速度 */
	UPROPERTY(EditAnywhere)
	float ZVelocity = 0.f;
};
