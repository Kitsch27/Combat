// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_CameraShake.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UAN_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	/** 镜头震动 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<UMatineeCameraShake> CamShake;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 1.0f ;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Epicenter = FVector::ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InnerRadius = 1500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OuterRadius = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Falloff = 1.f;
	
};
