// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Combo.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UAN_Combo : public UAnimNotify
{
	GENERATED_BODY()

public:
	/** 当前攻击走向的Combo */
	UPROPERTY(EditAnywhere)
	UAnimMontage* ComboMontage;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
