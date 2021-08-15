// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_LockAttack.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UAN_LockAttack : public UAnimNotify
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	bool bState;
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
