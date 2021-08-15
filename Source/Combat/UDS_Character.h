// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedStruct.h"
#include "UDS_Character.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API UUDS_Character : public UUserDefinedStruct
{
	GENERATED_BODY()
	
};

/** 角色状态 */
USTRUCT(BlueprintType)
struct FCharacterState
{
	GENERATED_BODY()

	/** 最大生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 1000.f;

	/** 生命值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 1000.f;

	/** 受击等级1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitLv1 = 10.f;

	/** 受击等级2 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HitLv2 = 100.f;

	/** 死亡状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;

	/** 倒地状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFallDown = false;

	/** 无敌状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInvincible = false;

	/** 霸体状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSuperArmor = false;

	/** 可以转向状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanRotate = true;
};

/** 击退类型 */
UENUM(BlueprintType)
enum class EHitKnockBackType:uint8
{
	E_HitPoint,	// 攻击碰撞点
	E_HitActor	// 攻击对象
};

/** 攻击伤害 */
USTRUCT(BlueprintType)
struct FAttackDamage
{
	GENERATED_BODY()

	/** 伤害值 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 100.f;

	/** 击退类型：根据攻击碰撞点，根据攻击actor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHitKnockBackType KnockBackType = EHitKnockBackType::E_HitActor;
	/** 击退冲量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockBackImpulse = 1.f;
	/** 是否覆盖Z轴击退方向 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bZKnockBackOverride = true;
	/** Z轴击退方向 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KnockBackImpulseZ = 0.f;
	
	/** 攻击顿帧慢发比例 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlomoRate = 0.1f;
	/** 攻击顿帧慢发时间 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlomoTime = 0.05f;

	/** 镜头震动 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCamera)
	TSubclassOf<UMatineeCameraShake> CamShake;
	
};