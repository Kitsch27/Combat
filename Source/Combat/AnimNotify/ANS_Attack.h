// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Combat/Character/CombatCharacter.h"
#include "ANS_Attack.generated.h"

UCLASS()
class COMBAT_API UANS_Attack : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	/** 获取武器插槽位置 */
	virtual TArray<FVector> GetLocationArr();

	/** 武器 */
	UPROPERTY()
	USkeletalMeshComponent* Weapon;

	/** 攻击检测方式 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseHitBox = false;

	/** 射线密度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Density = 5;

	/** 攻击伤害 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAttackDamage AttackDamage;

	/** 可以拼刀 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanParry = true;
	
	/** 武器插槽 */
	UPROPERTY()
	TArray<FName> SocketArr;
	/** 武器插槽位置 */
	UPROPERTY()
	TArray<FVector> LocationArr;
	/** 忽略的Actor */
	UPROPERTY()
	TArray<AActor*> IgnoreActorArr;
	/** 忽略的组件 */
	UPROPERTY()
	TArray<UPrimitiveComponent*> IgnoreCmpArr;


private:
	
	/** 处理碰撞结果 */
	virtual void DealHitResult(TArray<FHitResult>& HitResult);

	/** 击中破坏体*/
	virtual void HitDestructible(AActor* Actor, FVector Vector);

	/** 击中人物 */
	virtual void HitCharacter(FHitResult CurrentHitResult);
	
	UPROPERTY()
	ACombatCharacter* Character;
};
