// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Combat/CombatGameInstance.h"
#include "Combat/UDS_Character.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "CombatCharacter.generated.h"

UCLASS()
class COMBAT_API ACombatCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACombatCharacter();

	/** 武器 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	USkeletalMeshComponent* WeaponSkeletalMeshComponent;
	
	/** 伤害碰撞Box */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
	UBoxComponent* HitBox;

	/** 造成伤害 */
	virtual void UpdateHealth(float Value);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 改变人物时间膨胀
	void ChangeTimeDilation(float Rate);

	/** 改变世界时间膨胀 */
	void ChangeGlobalTimeDilation(float Rate) const;

	// 游戏实例
	UPROPERTY()
	UCombatGameInstance* CombatGameInstance;

	// 是否正在攻击
	bool bIsAttacking = false;
	// 在空中
	bool bIsInAir = false;
	// 在地面
	bool bIsOnGround = true;
	// 面向的方向
	FVector ForwardDirection;
	// 右侧方向
	FVector RightDirection;
	// 受攻击的方向
	FVector HitDirection;
	// 受攻击状态
	bool bIsGetHit = false;
	// 闪避状态
	bool bIsDodge = false;
	// 霸体状态
	
	
	// 计时器句柄
	FTimerHandle CountdownTimerHandle;
	FTimerHandle GlobalCountdownTimerHandle;
	// 移动组件
	UPROPERTY()
	UPawnMovementComponent* MovementComponent;

	/** 角色状态 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCharacterState CharacterState;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 是否可以移动
	virtual bool CanMove(float Rate) const;
	// 获取人物前方方向
	virtual FVector GetForwardDirection() const;
	// 获取人物向右方向
	virtual FVector GetRightDirection() const;
	// 收到攻击
	virtual void GetHit(AActor* AttackActor, FVector GetHitPoint, FVector GetHitDirection, FAttackDamage AttackDamage);
	// 进行攻击
	virtual void Hit(AActor* HitActor, FAttackDamage AttackDamage);
	// 获取攻击方向
	virtual FVector GetHitDirection();
	// 设置攻击状态
	virtual void SetAttacking(bool State);
	// 获取攻击状态
	virtual bool IsAttacking();
	// 设置受击状态
	virtual void SetGetHit(bool State);
	// 是否收到攻击
	virtual bool IsGetHit();

	// 闪避状态
	virtual void SetDodgeState(bool bDodgeState);

	// 人物顿帧
	void PauseFrame(float SlomoRate, float SlomoTime);

	/** 设置世界时间膨胀 */
	void SlomoGlobalTimeDilation(float SlomoRate, float SlomoTime);

	/** 获取人物状态 */
	virtual FCharacterState GetCharacterState();

	/** 人物倒地状态 */
	virtual void SetCharacterState(FCharacterState State);

	// 受伤特效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GetHit)
	UParticleSystem* GetHitParticle;
	// 受击音效
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GetHit)
	USoundBase* GetHitSound;
	
	// 受击Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GetHit)
	UAnimMontage* GetHitMontage;
	
	/** 死亡Montage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GetHit)
	UAnimMontage* DeadMontage;
	
};
