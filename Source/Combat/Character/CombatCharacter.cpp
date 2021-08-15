// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCharacter.h"

#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 武器
	WeaponSkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSMC"));
	WeaponSkeletalMeshComponent->SetupAttachment(GetMesh(), "weapon");
	
	// 伤害Box
	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(WeaponSkeletalMeshComponent, "HitBox");
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 修改空中制动和摩擦参数为与地面一致
	GetCharacterMovement()->BrakingDecelerationFalling = GetCharacterMovement()->BrakingDecelerationWalking;
	GetCharacterMovement()->FallingLateralFriction = GetCharacterMovement()->GroundFriction;

	Tags.Add("Character");
}

void ACombatCharacter::UpdateHealth(float Value)
{
	CharacterState.Health += Value;

	if (CharacterState.Health > 0.f)
	{
		CharacterState.bIsDead = false;
		return;
	}
	if (!CharacterState.bIsDead)
	{
		CharacterState.bIsDead = true;
		
		if (DeadMontage != nullptr)
		{
			UAnimInstance* AnimaInstance =  GetMesh()->GetAnimInstance();
			if (AnimaInstance)
			{
				// 播放死亡动画
				if (!AnimaInstance->Montage_IsPlaying(DeadMontage))
				{
					AnimaInstance->Montage_Play(DeadMontage);
				}
			}
		}
	}

}

// Called when the game starts or when spawned
void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 转换游戏实列
	CombatGameInstance = Cast<UCombatGameInstance>(GetGameInstance());

	// 移动组件
	MovementComponent = GetMovementComponent();
}

// 改变人物时间膨胀
void ACombatCharacter::ChangeTimeDilation(const float Rate)
{
	CustomTimeDilation = Rate;
}

void ACombatCharacter::ChangeGlobalTimeDilation(const float Rate) const
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), Rate);
}

// Called every frame
void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 计算人物方向
	const FRotator ActorRotation = GetActorRotation();
	const FRotator ActorYawRotation(0, ActorRotation.Yaw, 0);
	const FMatrix ActorMatrix = FRotationMatrix(ActorYawRotation);
	ForwardDirection = ActorMatrix.GetUnitAxis(EAxis::X);
	RightDirection = ActorMatrix.GetUnitAxis(EAxis::Y);
	
	// 更新在地面、空中的状态
	if (MovementComponent)
	{
		bIsInAir = MovementComponent->IsFalling();
		bIsOnGround = !bIsInAir;
	}


	// 速度、方向射线
	if (CombatGameInstance)
	{
		if (CombatGameInstance->bShowDebugDraw)
		{
			//调用画射线函数
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (ForwardDirection * 200), FColor::White, false, 0, 0, 5.f);
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (RightDirection * 200), FColor::White, false, 0, 0, 5.f);
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (GetVelocity() / GetVelocity().Size() * 200), FColor::Orange, false, 0, 0, 10.f);
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (ForwardDirection * (GetVelocity()|ForwardDirection) / GetVelocity().Size() * 200), FColor::Red, false, 0, 0, 10.f);
			DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + (RightDirection * (GetVelocity()|RightDirection)/ GetVelocity().Size() * 200), FColor::Green, false, 0, 0, 10.f);
		}
	}
	
}

// Called to bind functionality to input
void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// 返回人物是否可以移动
bool ACombatCharacter::CanMove(float Rate) const
{
	// 正在攻击、受到攻击、闪避、倒地时不可移动
	if (bIsAttacking || bIsGetHit || bIsDodge || CharacterState.bIsFallDown)
		return false;
	return true;
}

// 获取人物前方方向
FVector ACombatCharacter::GetForwardDirection() const
{
	return ForwardDirection;
}

// 获取人物右方方向
FVector ACombatCharacter::GetRightDirection() const
{
	return RightDirection;
}

/**
* 人物受到攻击
*
* @PARAM AttackActor 攻击者
* @PARAM GetHitPoint 攻击点
* @PARAM GetHitDirection 攻击方向
* @PARAM KnockBackDistance 击退距离
* @PARAM SlomoRate 顿帧时间膨胀率
* @PARAM SlomoTime 顿帧时间膨时长
*/ 
void ACombatCharacter::GetHit(AActor* AttackActor, const FVector GetHitPoint, const FVector GetHitDirection, const FAttackDamage AttackDamage)
{
	UE_LOG(LogTemp, Log, TEXT("%s : Get hit by %s"), *this->GetName(), *AttackActor->GetName());
	if (CombatGameInstance && CombatGameInstance->bShowDebugMsg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString(this->GetName() + " : Get hit by " + AttackActor->GetName()));
	}
	UpdateHealth(-AttackDamage.Damage);
	HitDirection = GetHitDirection;
	bIsGetHit = true;

	// 受击特效
	if (GetHitParticle)
	{
		const FRotator SpawnRotation = (-GetHitDirection.Rotation().RotateVector(FVector(0.f,0.f, -90.f))).Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GetHitParticle, GetHitPoint, SpawnRotation);
	}

	// 受击音效
	if (GetHitSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, GetHitSound, GetHitPoint);
	}

	// 受击顿帧
	PauseFrame(AttackDamage.SlomoRate, AttackDamage.SlomoTime);

	if (CharacterState.bIsDead)
		return;

	// 受击动画与击退
	if (!CharacterState.bIsSuperArmor && !CharacterState.bIsFallDown && AttackDamage.Damage >= CharacterState.HitLv1)
	{
		// 非霸体状态、非倒地状态以及伤害>=HitLv1 才后受击动画与击退
		CharacterState.bCanRotate = false;
		// 受击动画
		UAnimInstance* AnimaInstance =  GetMesh()->GetAnimInstance();
		if (AnimaInstance && GetHitMontage)
		{
			// 播放受击动画
			if (!AnimaInstance->Montage_IsPlaying(GetHitMontage))
			{
				AnimaInstance->Montage_Play(GetHitMontage);
			}

			// 根据受击方向，跳转对应的受击动画部分
			FName HitSection;
			
			if (bIsInAir)
			{
				// 空中
				HitSection = "Air";
			}
			else if (AttackDamage.Damage >= CharacterState.HitLv2)
			{
				// 重攻击
				HitSection = "Large";
			}
			else
			{
				const float ForwardHit = HitDirection | ForwardDirection;
				const float RightHit = HitDirection | RightDirection;
			
				if (fabs(ForwardHit) >= fabs(RightHit))
				{
					HitSection = ForwardHit > 0 ? "Back" : "Front";
				}
				else
				{
					HitSection = RightHit > 0 ? "Left" : "Right";
				}
			}
			// 无效时选择首个Section
			if (!GetHitMontage->IsValidSectionName(HitSection))
			{
				HitSection = GetHitMontage->GetSectionName(0);
			}
			AnimaInstance->Montage_JumpToSection(HitSection);
		}

		// 水平方向移动
		if (AttackDamage.KnockBackImpulse != 0)
		{
			FVector ImpulseDirection = GetHitDirection;
			if (AttackDamage.bZKnockBackOverride)
			{
				ImpulseDirection.Z = 0;
			}
			GetCharacterMovement()->AddImpulse(ImpulseDirection.GetSafeNormal() * AttackDamage.KnockBackImpulse * 1000, true);
		}

		if (AttackDamage.Damage < CharacterState.HitLv2)
		{
			// 垂直移动需要HitLv2以上
			return;
		}
		// 垂直方向移动
		if (AttackDamage.bZKnockBackOverride && AttackDamage.KnockBackImpulseZ != 0)
		{
			GetCharacterMovement()->Velocity.Z = 0;
			GetCharacterMovement()->AddImpulse(FVector::UpVector * AttackDamage.KnockBackImpulseZ * 1000, true);
			// LaunchCharacter(FVector::UpVector * AttackDamage.KnockBackImpulseZ * 1000, false, false);
		}
		
	}
}

/**
* 人物攻击
*
* @PARAM HitActor 攻击对象
* @PARAM SlomoRate 顿帧时间膨胀率
* @PARAM SlomoTime 顿帧时间膨时长
*/ 
void ACombatCharacter::Hit(AActor* HitActor, const FAttackDamage AttackDamage)
{
	if (CombatGameInstance && CombatGameInstance->bShowDebugMsg)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, FString(this->GetName() + " : Hit " + HitActor->GetName()));
	}

	// 攻击顿帧
	PauseFrame(AttackDamage.SlomoRate, AttackDamage.SlomoTime);
}

// 获取受击方向
FVector ACombatCharacter::GetHitDirection()
{
	return HitDirection;
}


/** 设置攻击状态 */
void ACombatCharacter::SetAttacking(const bool State)
{
	bIsAttacking = State;
}

/** 获取攻击状态 */
bool ACombatCharacter::IsAttacking()
{
	return bIsAttacking;
}

/** 设置受击状态 */
void ACombatCharacter::SetGetHit(const bool State)
{
	bIsGetHit = State;
}

/** 获取受击状态 */
bool ACombatCharacter::IsGetHit()
{
	return bIsGetHit;
}

/** 设置闪避状态 */
void ACombatCharacter::SetDodgeState(const bool bDodgeState)
{
	bIsDodge = bDodgeState;
}


/**
* 人物顿帧
*
* @PARAM SlomoRate 顿帧时间膨胀率
* @PARAM SlomoTime 顿帧时间膨时长
*/ 
void ACombatCharacter::PauseFrame(const float SlomoRate, const float SlomoTime)
{
	if (GetWorld() == nullptr || UGameplayStatics::GetGlobalTimeDilation(GetWorld()) != 1.f)
		return;
	ChangeTimeDilation(SlomoRate);
	GetWorldTimerManager().SetTimer(CountdownTimerHandle, FTimerDelegate::CreateUObject(this, &ACombatCharacter::ChangeTimeDilation, 1.0f), SlomoTime, true, -1);
}

/** 设置世界时间膨胀 */
void ACombatCharacter::SlomoGlobalTimeDilation(const float SlomoRate, const float SlomoTime)
{
	if (GetWorld() == nullptr)
		return;
	ChangeGlobalTimeDilation(SlomoRate);
	GetWorldTimerManager().SetTimer(GlobalCountdownTimerHandle, FTimerDelegate::CreateUObject(this, &ACombatCharacter::ChangeGlobalTimeDilation, 1.0f), SlomoTime, true, -1);
}

/** 获取人物状态 */
FCharacterState ACombatCharacter::GetCharacterState()
{
	return CharacterState;
}

/** 人物倒地状态 */
void ACombatCharacter::SetCharacterState(FCharacterState State)
{
	CharacterState = State;
}
