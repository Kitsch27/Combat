// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SetState.generated.h"

UENUM(BlueprintType)
enum class EState:uint8
{
	E_True UMETA(DisplayName = "True"),
	E_False UMETA(DisplayName = "False"),
	E_NoChange UMETA(DisplayName = "NoChange"),
};

UCLASS()
class COMBAT_API UAN_SetState : public UAnimNotify
{
	GENERATED_BODY()
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/** 攻击状态 */
	UPROPERTY(EditAnywhere)
	EState AttackState = EState::E_NoChange;
	
	/** 受攻击状态 */
	UPROPERTY(EditAnywhere)
	EState GetHitState = EState::E_NoChange;
	
	/** 闪避状态 */
	UPROPERTY(EditAnywhere)
	EState DodgeState = EState::E_NoChange;

	/** 倒地状态 */
	UPROPERTY(EditAnywhere)
	EState FallDownState = EState::E_NoChange;

	/** 无敌状态 */
	UPROPERTY(EditAnywhere)
	EState InvincibleState = EState::E_NoChange;

	/** 霸体状态 */
	UPROPERTY(EditAnywhere)
	EState SuperArmorState = EState::E_NoChange;

	/** 可以转向状态 */
	UPROPERTY(EditAnywhere)
	EState CanRotateState  = EState::E_NoChange;

	/** 胶囊体高度 */
	UPROPERTY(EditAnywhere, meta=(ClampMin="0", UIMin="0"))
	float CapsuleHalfHeight  = 88.f;
	
};
