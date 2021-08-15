// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_LaunchCharacter.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UAN_LaunchCharacter : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LaunchVelocity = FVector::UpVector;
	
};
