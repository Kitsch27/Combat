// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIC_Enemy.h"
#include "CombatCharacter.h"
#include "CombatEnemy.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatEnemy : public ACombatCharacter
{
	GENERATED_BODY()

public:
	ACombatEnemy();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	/** 攻击 */
	virtual void Attack(UAnimMontage* AttackMontage);

	/** 被拼刀 */
	virtual void BeParried(AActor* AttackActor, const FVector GetHitPoint, const FVector GetHitDirection, const FAttackDamage AttackDamage);

	virtual void UpdateHealth(float Value) override;

	/** 拼刀特效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GetHit)
	UParticleSystem* BeParriedParticle;
	/** 拼刀音效 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GetHit)
	USoundBase* BeParriedSound;

protected:
	/** 是否可以看到Player */
	bool bCanSeePlayer;

	/** 控制器 */
	UPROPERTY()
	AAIC_Enemy* EnemyController;

	/** 黑板数据 */
	UPROPERTY(BlueprintReadOnly, Category = AI)
	class UBlackboardData* BlackBoardData;
};
