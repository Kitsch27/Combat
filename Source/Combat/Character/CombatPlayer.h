// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <deque>
#include "CombatCharacter.h"
#include "Combat/DataTable/DT_Montage.h"
#include "Engine/DataTable.h"
#include "CombatPlayer.generated.h"

/**
 * 
 */
UCLASS()
class COMBAT_API ACombatPlayer : public ACombatCharacter
{
	
	GENERATED_BODY()

	/** 摄像机的弹簧臂 */
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraSpringArm;

	/** 摄像机 */
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FollowCamera;

public:
	ACombatPlayer();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 按下普通攻击 */
	virtual void PressedNormalAttack();
	/** 释放普通攻击 */
	virtual void ReleasedNormalAttack();

	/** 镜头水平旋转 */
	virtual void CameraRotationX(float Rate);

	/** 镜头垂直旋转 */
	virtual void CameraRotationY(float Rate);

	/** 输入前后移动 */
	virtual void InputMoveForward(float Rate);

	/** 输入左右移动 */
	virtual void InputMoveRight(float Rate);
	/** 方向输入缓存 */
	void MoveInputBuffering(float PreRate, float Rate, FVector InputDirection);

	/** 锁定 */
	virtual void Lock();
	/** 释放锁定 */
	virtual void ReleaseLock();

	/** 闪避 */
	virtual void Dodge(FMontageInfo* MatchCommand);

	/** 获得锁定状态 */
	virtual bool GetLockState() const;

	/** 是否可以移动 */
	virtual bool CanMove(float Rate) const override;

	/** 获取范围内可以锁定项列表 */
	virtual TArray<AActor*> GetLockArr();

	/** 执行指令 */
	void ExecutionCommand(FMontageInfo* MatchCommand);

	/** 攻击 */
	void Attack(FMontageInfo* MatchCommand);

	/** 设置ComboMontage */
	virtual void SetComboMontage(UAnimMontage* NewMontage);

	/** 恢复攻击Montage */
	virtual void ResetAttackMontage();

	/** 摄像机震动 */
	virtual void CameraShake(TSubclassOf<UCameraShakeBase> CamShake, float Scale);

	/** 受到攻击 */
	virtual void GetHit(AActor* AttackActor, FVector GetHitPoint, FVector GetHitDirection, FAttackDamage AttackDamage) override;

	/** 攻击命中 */
	virtual void Hit(AActor* HitActor, FAttackDamage AttackDamage) override;

	/** 改变可以切换攻击的状态 */
	virtual void SetCanChangeAttack(bool bState);

	/** 按下跳跃 */
	virtual void PressedJump();

	/** 释放跳跃键 */
	virtual void ReleasedJump();

	/** 闪避 */
	virtual void PressedDodge();

	/** 跟随玩家的镜头 */
	UPROPERTY(EditAnywhere, Category = PlayerCamera)
	float BaseCameraRotationRateX = 100.f;
	
	/** 镜头垂直移动速率 */
	UPROPERTY(EditAnywhere, Category = PlayerCamera)
	float BaseCameraRotationRateY = 100.f;
	
	/** 镜头垂直移动范围下限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCamera)
	float ViewPitchRangeMin = -25.f;
	
	/** 镜头垂直移动范围上限 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCamera)
	float ViewPitchRangeMax = 20.f;

	/** 默认镜头旋转角度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerCamera)
	FRotator DefaultControllerRotation = FRotator(-25.f, 0.f, 0.f);

	/** 移动输入下限 */
	UPROPERTY(EditAnywhere, Category = Move)
	float MinIgnoreMoveInput = 0.2f;

	/** 移动输入上限 */
	UPROPERTY(EditAnywhere, Category = Move)
	float MaxIgnoreMoveInput = 1.f;

	/** 蓄力时间 */
	UPROPERTY(EditAnywhere, Category = Attack)
	float ChargeAttackTime = 0.5f;
	/** 输入缓存最大帧时长 */
	UPROPERTY(EditAnywhere, Category = Attack)
	int MaxInputBufferSize = 60;

	/** 锁定范围 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Lock)
	float LockDistance = 2000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DataTable)
	UDataTable* MontageDT;

 protected:
	virtual void BeginPlay() override;

	/** 解析输入指令 */
	virtual void ParseInputBuffering();
	/** 匹配指令列表 */
	virtual FMontageInfo* MatchCommandList();

	/** 读取MontageDataTable，存入对应数组 */
	virtual void ReadMontage();

	/** debug信息*/
	virtual void ShowDebugMsg();
	virtual void ShowDebugDraw();

	

	// /** 蓄力普通攻击 */
	// virtual void ChargeNormalAttack();
	// /** 释放蓄力普通攻击 */
	// virtual float ReleasedChargeAttack();

	/** 默认攻击Montage */
	UPROPERTY()
	UAnimMontage* DefaultAttackMontage;
	/** 当前攻击Montage */
	UPROPERTY()
	UAnimMontage* ComboMontage;
	/** 当前技能Montage */
	UPROPERTY()
	UAnimMontage* SkillAMontage;

	/** 所有Montage */
	TMap<EWeaponType, TArray<FMontageInfo*>> AllMontageInfoMap;
	/** 所有技能Montage */
	TMap<EWeaponType, TArray<FMontageInfo*>> AllSkillMap;
	/** 当前武器Montage */
	TArray<FMontageInfo*> CurrentMontageInfoArr;
	/** 当前武器技能Montage */
	TArray<FMontageInfo*> CurrentSkillArr;
	/** 当前武器 */
	EWeaponType CurrentWeaponType = EWeaponType::E_Sword;

	/** 锁定的对象 */
	UPROPERTY(BlueprintReadOnly)
	AActor* LockActor;

	/** Player控制器 */
	UPROPERTY()
	APlayerController* PlayerController;

	/** 输入的前方方向 */
	FVector InputForwardDirection;
	/** 输入的右方方向 */
	FVector InputRightDirection;

	/** 输入缓存 */
	std::deque<std::deque<EInputBufferingType>> InputBuffering; // 输入缓存
	std::deque<EInputBufferingType> CurrentInputBuffering;	// 当前帧输入
	float PreForwardRate = 0;	// 前一帧前后方向输入量
	float PreRightRate = 0;		// 前一帧左右方向输入量
	
	/** 普通攻击已蓄力时间 */
	float CurrentChargeNormalAttackTime = 0.0f;
	/** 是否开始蓄力 */
	bool bIsStartChargeNormalAttack = false;
	/** 是否开启锁定 */
	bool bIsLocking = false;
	/** 是否可以切换攻击 */
	bool bCanChangeAttack = true;

private:
	virtual void PressedToGetHit();
	virtual void PressedToRecoveryHealth();
	
};
